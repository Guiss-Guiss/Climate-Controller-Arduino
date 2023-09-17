
Written by Guillaume Ste-Marie
GNU GENERAL PUBLIC LICENSE, see the LICENSE file for details.
This sketch allows to control the climate in a room, a greenhouse, a grow tent,
an aquariun, a vivarium, or any other enclosure. It can also be used to control
the fermentation temperature of beer or wine. It can be used to control your world.
It is using a variety of sensing and controlling devices.
It sets up the necessary pins, libraries, and variables to read the
temperature, humidity, and control the relays to power the heater,
exhaust fan or cooling, humidifier, dehumidifier, and lights.
The user can access a menu to set the desired temperature, humidity,
and lighting times. It uses the EEPROM memory of the DS3231 RTC to store the settings,
a DS3231 RTC module to keep track of time, a dht22 temperature and humidity sensor
to read temperature and relative humidity. It uses a 20x4 LCD screen to display the current time.
It uses 5 momentary switches to navigate the menu and set the desired values.
It uses 5 relays to control the heater, exhaust fan or cooling, humidifier, dehumidifier, and lights.
Visit https://www.guillaumeguiss.com or email at controller@guillaumeguiss.com for more information.
Tested on Arduino Uno, Arduino Mega, and Arduino Nano.
