NodeJS - XBee - jQuery - Arduino Tank (still working on a name)
===

### Required 
nodeJS
node-serial

Arduino Uno
Motor Shield
Xbee Shield and Xbee pair
Other hardward for a tank

### To Run
attach the Xbee to the USB port
change var comPort = '/dev/tty.usbserial-A900fwHn' to your comport

Run node server with..
	node page.js

should result in the following message

	done init
	Server Running on 8084
    comm port ready
    connect - localhost:8004/index.html

 A typical error results when you get the comm port wrong or the radio transmittor is not attached.

    $ node page.js 
	done init
	Server Running on 8084

	events.js:72
    throw er; // Unhandled 'error' event
              ^
	Error: Cannot open /dev/tty.usbserial-A900fwHn





### To Do

 - Ready the documenation, add tags to the code, add event handling and the science package to the code.
 - the readme itself has format issues so I need to learn how to format to my liking.
 - event handling for various error types.
 - Engine power updates to the robot.
 - Engine Impluse delay to the robot.
 


