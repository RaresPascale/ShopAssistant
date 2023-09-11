# ShopAssistant
(https://github.com/RaresPascale/ShopAssistant/assets/98171722/4cf2393d-c47d-4124-bd43-81913e80299e)

# As a summer project, me and my colleague created a robot, more exactly a smarter shopping cart.

# On a 4 wheeled mobile platform, controllable via an IR remote, we mounted a shopping basket with the SparkFun M6e Nano, simultaneous RFID reader.

# The mobile part is developed on the Arduino Nano board, whereas the RFID Reader is integrated using Arduino Mega 2560

# The sensor inside the shopping cart is able to simultaneously read the RFID tags that we placed on some products. Moreover, the read data is written on a SD Card.

# The data is written on the SD card, in the following way: 
  * At every 3 seconds, a new reading is performed by the sensor, and a new line is written
  * At the start of each line, using the RTC module, the exact date and time is placed.
  * Next, the id of each tag is written only once( the RFID reader, can read a tag       
    multiple times in a second, but we found a way to write it only once in the memory).![Poza prezentare1]


# Finally, we can insert the SD card on a PC, and using a desktop app, created using Python, we can view the data in a nicer way.


# Modules and sensors used:
  ~ Mobile part: Arduino Nano, L298N H-Bridge, 4 DC Motors, IR sensor;
  ~ Tag Reading: Arduino Mega 2560, M6e Nano Simultaneous Reader by Spark Fun, RTC Module DS 1307, SD Card Module;
  ~ Power: 2 4.2V Li-Ion Batteries, LM2596 voltage dropper;


# Communication protocols used:
  ~ I2C: for RTC Module
  ~ SPI: for SD card
  ~ UART: for RFID reader
  ~ One wire protocol: IR sensor



