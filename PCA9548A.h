#ifndef _PCA9548A_H_
#define _PCA9548A_H_

#include "application.h"

class PCA9548A
{
public:
  //Constructor - Creates an PCA9548A object
  //Requires absolute address of PCA9548A on i2c bus + 
  //uC output pin for resetting the PCA9548A
  PCA9548A(uint8_t _i2cAddress, int _resetPin);
  
  //Should be called at startup or whenever IC should be brought
  //back to POR condition, such as if an i2c peripheral has become troublesome.
  //This will assert the reset pin `resetPin` low, 
  //removing devices on all i2c channels from the bus. 
  void reset();

  //Checks to see the state of a certain channel (0 to NUM_CHANNELS - 1 )
  //True means a channel is currently enabled (allowed on the group i2c bus)
  bool isEnabled(uint channel);

  //Change the state of an individual channel (0 to NUM_CHANNELS - 1 )
  //on the PCA9548A. `True` will allow this channel to join the group i2c bus
  //returns back something other than 0 if something went wrong when sending 
  //state to the PCA9548A IC over i2c
  int changeState(uint channel, bool newState);

protected:
  //Write our local channelState variable to the PCA9548A IC over i2c
  int transmitState(); 

  //Const member variables set in constructor
  const uint8_t i2cAddress;
  const int resetPin; 

  //Number of channels currently enabled. 
  //Changing this will allow compatibility with all PCA954x variants
  const static unsigned int NUM_CHANNELS = 8;

  //Holds state telling which i2c channels are currently enabled
  bool channelState[NUM_CHANNELS];
};

#endif //_PCA9548A_H_