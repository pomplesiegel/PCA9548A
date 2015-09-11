#include "application.h"
#include "PCA9548A.h"

SYSTEM_MODE(MANUAL);

//PCA9548A at its lowest i2c address, 0x70
//RESET connection going from Photon's D6 output to 
//PCA9548A's `_RESET` input
PCA9548A i2cSwitch(0x70, D6); 

void setup() 
{
  Serial.begin(9600);
  
  Serial.println("PCA9548A Test!");

  Wire.begin();

  //Reset the PCA9548A back to startup condition
  i2cSwitch.reset();

  //Turn on channel 0, or whichever you would like, up to 7
  i2cSwitch.changeState(0, true);

  //Only now can you communicate with a peripheral on the other side of this switch!
}


void loop()
{
  //Communicate w/ some peripheral here
}