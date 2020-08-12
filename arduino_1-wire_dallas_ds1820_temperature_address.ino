/* 
 * This Arduino sketch lists the addresses of all DS18* and DS2* temperature sensors 
 * on a single wire (1-Wire), fetches their addresses and outputs their temperature readings to the
 * specified serial port in degree Celsius and Fahrenheit.
 *
 * The following libraries are used:
 * - OneWire library: https://www.pjrc.com/teensy/td_libs_OneWire.html
 * - DallasTemperature library: https://github.com/milesburton/Arduino-Temperature-Control-Library
 *
 * Author: Pascal Werkl
 * Url: https://github.com/pasce
 * Date: 12.08.2020
 *  
 * Prerequirements:
 * - Your sensors are wired correctly according to the 1-wire specification (Normal mode or parasite mode)
 *    - You use one pull up resistor (4.7k ohm) to be able to read from the sensors etc.
 * - Your circuit is hooked up to an Arduino board on digital PIN2
 *    - GND <-> GND, Data <-> PIN2, VCC <-> 3,3 Volt or 5 Volt.
 * 
 * Caveats:
 * - This sketch is functional but has not been thoroughly tested
 * - It is suitable for demonstration purposes only
 * - It might find all different types of 1-wire sensors on the line but can deal with temperature sensors only 
 * - The code only finds a maximum number of sensors which is defined by: MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND
 *   - But that's not problem as you can set the number as high as you want. Restricted by the amount of available memory
 * - Because the code was written for demonstration purposes it is very verbose and unoptimized
 * 
 * Process:
 * - Perform basic setup
 * - Get the number of devices on the bus
 * - Check if the sensors are running in parasite mode
 * - Search for 1-wire devices on the bus and get their addresses
 * - Populate the array with the addresses [temp_sensors]
 * - Set the precision of the sensors [TEMPERATURE_PRECISION]
 * - |-> Perform sensor readings and get the results in degree Celsius and Fahrenheit
 * - | Iterate over all sensors and print the addresses and temperature readings
 * - | Pause for 5 seconds or so...
 * - |- Repeat
 * 
 * To avoid dynamic memory allocation the user-defined preprocessor variable MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND
 * is used. However, this approach limits the number of devices that can be found by the script. It's 
 * a little ugly but does the job. You will need 8 bytes of global memory to store a single
 * device address. eg: MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND = 10 -> 10*8 = 80 bytes.
 * 
 * This example uses source code from the DallasTemperature library:
 * https://github.com/milesburton/Arduino-Temperature-Control-Library
 */

/*
 * Both libraries can be installed through the Arduino IDE
 * Go to: 
 * Tools -> Manage libraries...
 * 
 * Search for: 'OneWire'
 * install version 2.3.5 or above
 * 
 * Search for: 'DallasTemperature'
 * install version 3.8.0 or above
 */
 
// DS1820 and DS18S20 have no resolution configuration register -> They don't support resolution changes via the setResolution()
// call of the library. In this sketch it is used anyway for all sensor models because it does not seem to have any negative effects.

#include <OneWire.h>
#include <DallasTemperature.h>

#define SERIAL_PORT_BOUD_RATE 9600 //<- change if you use a different boud rate for serial communication

// sensors are connected to digital PIN2 of the arduino
#define ONE_WIRE_BUS 2 //<- change the pin number if you use a different pin on your arduino

// specify the max. number of discoverable sensors. 8 Byte per sensor needed.
#define MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND 10 //<- change if you want more sensors to be discovered

// most sensors support 9-12 bit precission. Lower resolution means faster readings
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// statically allocate global memory to hold the address of the devices. 8 byte per sensor.
DeviceAddress temp_sensors[MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND];

// holds the actual number of found sensors on the wire.
int sensorCount;

void setup(void)
{
  Serial.begin(SERIAL_PORT_BOUD_RATE);
  Serial.println("Dallas DS18* and DS2* temperature sensor example");

  sensors.begin();
  sensorCount = sensors.getDeviceCount();
  
  Serial.print("Number of sensors found: ");
  Serial.println(sensorCount);

  if (sensorCount > MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND) {
    Serial.print("ERROR: MAX_NUMBER_OF_POSSIBLE_SENSORS_FOUND is too small. Set it at least to: ");
    Serial.println(sensorCount);

  }
  
  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // search for 1-wire devices. always perform a reset before starting a search
  oneWire.reset_search();

  for(int i=0;i<sensorCount;i++) {
    
    if (!oneWire.search(temp_sensors[i])) {
      Serial.print("Unable to find address for sensor: ");
      Serial.println(i);
    }
    else {
      Serial.println();
      Serial.print("Sensor found with address: ");
      printAddress(temp_sensors[i]);
      Serial.println();

      Serial.print("Setting resolution for sensor: ");
      printAddress(temp_sensors[i]);
      Serial.print(" to: ");
      Serial.print(TEMPERATURE_PRECISION);
      Serial.println(" bit");
      
      sensors.setResolution(temp_sensors[i], TEMPERATURE_PRECISION);
      delay(100);

      Serial.print("Getting resolution for sensor ");
      printAddress(temp_sensors[i]);
      Serial.print(": ");
      Serial.print(sensors.getResolution(temp_sensors[i]), DEC);
      Serial.println(" bit");
      
    }
  }
  Serial.println();
} 

void loop(void)
{ 
  // request temperature readings for all sensors (magic is done by the library)
  sensors.requestTemperatures(); 
 
  // output the temperature readings of all sensors in degree Celius and Fahrenheit
  // conversions are done by the library
  for(int i=0;i<sensorCount;i++) {
    Serial.print("Sensor: ");
    Serial.print(i);
    Serial.print(" @");
    printAddress(temp_sensors[i]);
    Serial.println();
    Serial.print("Temperatur: "); 
    printValue(sensors.getTempCByIndex(i), "°C");
    Serial.print("Temperatur: ");
    printValue(sensors.getTempFByIndex(i), "°F");
  }
  Serial. println();
 
  delay(5000); // pause the readings for 5 seconds
}

// function to print a device address
// https://github.com/milesburton/Arduino-Temperature-Control-Library
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void printValue(float value, String text)
{
  Serial.print(value);
  Serial.println(text);
} 
