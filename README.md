# Temperature Sensor (DS18x20) Sketch Example for Arduino

This Arduino sketch **lists the addresses** of all 1-wire enabled *DS18x and DS2x* temperature sensors on a single bus (via the [**1-Wire serial protocol**](https://en.wikipedia.org/wiki/1-Wire "1-Wire serial protocol")), fetches their addresses and **outputs their temperature readings** to the specified serial port in degree Celsius and Fahrenheit.

This sketch can be used to **check the functionality** of a circuit or to list the **hardware addresses** of the hooked up **temperature sensors**.
 
## Supported temperature sensors
- **DS18S20MODEL** 	also DS1820
- **DS18B20MODEL**  also MAX31820
- **DS1822MODEL**
- **DS1825MODEL**
- **DS28EA00MODEL**

## The following libraries are used for the sake of convenience
- [**OneWire library**](https://www.pjrc.com/teensy/td_libs_OneWire.html "OneWire library")
- [**DallasTemperature library**](https://github.com/milesburton/Arduino-Temperature-Control-Library "DallasTemperature library")

Both libraries can be installed through the Arduino IDE.

 - Go to **Tools** -> **Manage libraries...**
	 - Search for: **OneWire**
	 - Install version **2.3.5 or above**
	- Search for: **DallasTemperature**
	- Install version **3.8.0 or above**


## Prerequirements
- Your **sensors are wired correctly** according to the 1-wire bus system ([Normal power mode](https://www.tweaking4all.com/wp-content/uploads/2014/03/ds18b20-normal-power-400x184.jpg "Normal mode") or [parasite power mode](https://www.tweaking4all.com/wp-content/uploads/2014/03/ds18b20-normal-power-400x184.jpg "parasite mode"))
- Your circuit is hooked up to an Arduino board on **digital input PIN2**
- Connect: GND <-> GND, DATA <-> PIN2, VCC <-> 3,3V or 5V.
	- You use one **pull up resistor** (4.7k ohm)  between **Data and VCC** to be able to read from the sensors etc.

If you **need help with the wiring** check out this site: [**How to interface multiple DS18B20 on a single bus**](https://lastminuteengineers.com/multiple-ds18b20-arduino-tutorial/ "How to interface multiple DS18B20 on a single bus") 

## Caveats
 - This sketch is functional but **has not been thoroughly tested**
 - It is suitable for demonstration purposes only
 - It might **find all different types** of 1-wire sensors on the line but can **deal with temperature sensors only** 
 - The code **only finds a maximum number** of sensors which is defined by *MAX\_NUMBER\_OF\_POSSIBLE\_SENSORS\_FOUND*
	 * ...but that's not problem becauase you can set the number as high as you want. Restricted by the amount of available memory
 - Because the code was written for demonstration purposes **it is very verbose** and unoptimized

## Program Flow
 - Perform basic setup
 - Get the number of devices on the bus
 - Check if the sensors are running in parasite mode
 - Search for 1-wire devices on the bus and get their addresses
 - Populate the array with the addresses [temp_sensors]
 - Set the precision of the sensors [TEMPERATURE_PRECISION]
 - |--> Perform sensor readings and get the results in degree Celsius and Fahrenheit
 - | Iterate over all sensors and print the addresses and temperature readings
 - | Pause for a few seconds
 - |-- Repeat

## Additional Info
To avoid dynamic memory allocation the user-defined preprocessor variable **MAX\_NUMBER\_OF\_POSSIBLE\_SENSORS\_FOUND**
is used.

However, this approach limits the number of devices that can be found by the script. It's a little ugly but does the job.
You will need **8 bytes of global memory to store a single device address**.

eg: **MAX\_NUMBER\_OF\_POSSIBLE\_SENSORS\_FOUND = 10** -> 10*8 = **80 bytes**.
 

This example uses source code from the [**DallasTemperature library**](https://github.com/milesburton/Arduino-Temperature-Control-Library).

**DS1820 and DS18S20 have no 'resolution configuration register'** This means, they don't support resolution changes via the *setResolution()* call of the library. In this sketch it is used anyway for all sensor models because it does not seem to have any negative effects.

## Example Output
The sketch displays **all temperature sensors** and **their addresss** and outputs **their temperature readings** in Celsius and Fahrenheit.

![Example output](https://raw.githubusercontent.com/pasce/Arduino-DS18x20-DS28x-Temperature-Sensor-EX/master/assets/arduino_ide_dallas_ds1820_temperature_1wire_output.png)

<pre>
Dallas DS18 and DS2 temperature sensor example
Number of sensors found: 3
Parasite power is: ON

Sensor found with address: 0x10 0x1 0xCB 0xC2 0x2 0x8 0x0 0x99
Setting resolution for sensor: 1001CBC202080099 to: 12 bit
Getting resolution for sensor 1001CBC202080099: 12 bit

Sensor found with address: 0x10 0xCB 0x61 0xDE 0x2 0x8 0x0 0x6D
Setting resolution for sensor: 10CB61DE0208006D to: 12 bit
Getting resolution for sensor 10CB61DE0208006D: 12 bit

Sensor found with address: 0x10 0x9B 0xDA 0xDE 0x2 0x8 0x0 0x7C
Setting resolution for sensor: 109BDADE0208007C to: 12 bit
Getting resolution for sensor 109BDADE0208007C: 12 bit

Sensor: 0 @1001CBC202080099
Temperatur: 24.50°C
Temperatur: 76.10°F
Sensor: 1 @10CB61DE0208006D
Temperatur: 25.31°C
Temperatur: 77.56°F
Sensor: 2 @109BDADE0208007C
Temperatur: 24.31°C
Temperatur: 75.76°F
</pre>

## Contribute
If you found bugs or/and put extra work into the sketch, **please contribute** so that others can benefit and learn from it.

**Thank you**.