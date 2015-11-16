/**********************************************************************

Sensor.cpp
COPYRIGHT (c) 2013-2015 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino Uno 

**********************************************************************/
/**********************************************************************

DCC++ BASE STATION supports Sensor inputs that can be connected to any Aruidno Pin 
not in use by this program.  Sensors can be of any type (infrared, magentic, mechanical...).
The only requirement is that when "activated" the Sensor must force the specified Arduino
Pin LOW (i.e. to ground), and when not activated, this Pin should remain HIGH (e.g. 5V),
or be allowed to float HIGH if use of the Arduino Pin's internal pull-up resistor is specified.

To ensure proper voltage levels, some part of the Sensor circuitry
MUST be tied back to the same ground as used by the Arduino.

The Sensor code below utilizes exponential smoothing to "de-bounce" spikes generated by
mechanical switches and transistors.  This avoids the need to create smoothing circuitry
for each sensor.  You may need to change these parameters through trial and error for your specific sensors.

All Sensors should be defined in a single global array declared in DCCpp_Uno.ino using the format:

  Sensor(ID, PIN, PULLUP)

  ID: a unique integer ID (0-32767) for this sensor
  PIN: the Arudino Pin used to read the sensor
  PULLUP: HIGH (1) to enable this Pin's internal pull-up resistor, LOW (0) to disable the internal pull-up resistor 

All Sensors defined in the global array are repeatedly and sequentially checked within the main loop of this sketch.
If a Sensor Pin is found to have transitioned from a HIGH state to a LOW state, the following serial message is generated:

  <Q ID>

  where ID is the ID of the Sensor that was triggered

No message is generated when a Sensor Pin transitions back to a HIGH state from a LOW state. 

**********************************************************************/

#include "DCCpp_Uno.h"
#include "Sensor.h"

///////////////////////////////////////////////////////////////////////////////
  
Sensor::Sensor(int snum, int pin, int pullUp){
  this->pin=pin;
  this->snum=snum;
  signal=1;
  active=false;
  pinMode(pin,INPUT);         // set mode to input
  digitalWrite(pin,pullUp);   // don't use Arduino's internal pull-up resistors for external infrared sensors --- each sensor must have its own 1K external pull-up resistor
  nSensors++;
} // Sensor::Sensor

///////////////////////////////////////////////////////////////////////////////
  
void Sensor::check(){    
  signal=signal*(1.0-SENSOR_DECAY)+digitalRead(pin)*SENSOR_DECAY;

  if(!active && signal<0.5){
    active=true;
    Serial.print("<Q");
    Serial.print(snum);
    Serial.print(">");
  } else if(active && signal>0.99){
    active=false;
  }
} // Sensor::check

///////////////////////////////////////////////////////////////////////////////

int Sensor::nSensors=0;
