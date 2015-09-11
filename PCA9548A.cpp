#include "PCA9548A.h"

// reads the bit of a number
#define bitRead(number, bit) ( (number >> bit) & 0x01 )
// writes a 1 to a bit of a number
#define bitSet(number, bit) ( number |= (1UL << bit) )
// writes a 0 to a bit of a number
#define bitClear(number, bit) ( number &= ~(1UL << bit) )
// writes a specified value (0 or 1) to a bit of a number
#define bitWrite(number, bit, value) ( value ? bitSet(number, bit) : bitClear(number, bit) )

//Constructor - Creates an PCA9548A object
//Requires absolute address of PCA9548A on i2c bus + 
//uC output pin for resetting the PCA9548A
PCA9548A::PCA9548A(uint8_t _i2cAddress, int _resetPin) : 
  i2cAddress(_i2cAddress), resetPin(_resetPin)
{
  //Declare the stated uC reset output pin as a GPO
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH); //Make sure it's not asserted!
}

//Should be called at startup or whenever IC should be brought
//back to POR condition, such as if an i2c peripheral has become troublesome.
//This will assert the reset pin `resetPin` low, 
//removing devices on all i2c channels from the bus. 
void PCA9548A::reset()
{
  Serial.println("Resetting i2c switch");
  //clear our state array
  for(uint i=0; i < NUM_CHANNELS; i++)
    channelState[i] = false;

  //Assert reset pin low for 5ms to disable all channels
  digitalWrite(resetPin, LOW);
  delayMicroseconds(500);
  digitalWrite(resetPin, HIGH);
}

//Checks to see the state of a certain channel (0 to NUM_CHANNELS - 1 )
//True means a channel is currently enabled (allowed on the group i2c bus)
bool PCA9548A::isEnabled(uint channel)
{
  //Watch for out of array bounds
  if(channel > NUM_CHANNELS - 1)
    return 0;
  //otherwise, return the current state for this channel
  return channelState[channel];
}

//Change the state of an individual channel (0 to NUM_CHANNELS - 1 )
//on the PCA9548A. `True` will allow this channel to join the group i2c bus
//returns back something other than 0 if something went wrong when sending 
//state to the PCA9548A IC over i2c
int PCA9548A::changeState(uint channel, bool newState)
{
  //Watch for out of array bounds
  if(channel > NUM_CHANNELS - 1)
    return -1;

  //Change our local state to reflect new value
  channelState[channel] = newState;

  //Transmit our entire channel state to the IC
  //Use Wire.endTransmission's success flag as return type
  return transmitState();
}


//Write our local state variable to the PCA9548A IC
int PCA9548A::transmitState()
{
  //clear our word to transmit to the IC
  uint8_t stateWord = 0x00; 

  for(uint i = 0; i < NUM_CHANNELS; i++)
    bitWrite( stateWord, i, channelState[i] );

  Serial.println("Channel state binary -> i2c switch: " + String(stateWord, BIN) );

  Wire.beginTransmission(i2cAddress);
  Wire.write(stateWord); //Write our newly created stateWord to the IC
  return Wire.endTransmission(); //Actually perform the i2c transaction
}