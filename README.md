NodeRover XR1
==

![The Rover](/images/spaceapps.png )
![The Rover](/images/nasa_logo.png)

[The Rover that could](https://2014.spaceappschallenge.org/project/the-little-rover-that-could/)


### What is it?
This is an experiment where nodeJs acts as a middleware between the UI controller and other services on the back end while the user drives the robot.  The nodeJS server could also attach other services to aid the rover, collect data, etc.

A combination of jQuery, nodeJs, Xbee and Arduino. This drives a custom built robot through web page interface around its environment.  This is still experimental and code changes often.

This is my first project I am sharing on GitHub, so please be patient.

_____
## The Rover

![The Rover](/images/XR1.JPG)

#### Radio transceiver.
![Xbee Transceiver ](/images/xBee.JPG)


## The Interface
![Web Interface ](/images/nodeRoverControls.jpg)

Left to Right.
* Driving panel
* Engine power (left and right engine)
* Ultra Sonic radar map

Four Quadrants of data around the directional arrows
* Top left is range data
* Top right is the last robot command
* Bottom left will be extra sensor data
* Bottom right robot settings: left engine, right engine, command delay, range.

______
### Required 

You need a serial device attached in order to run the nodeJS server otherwise the Serial code will not run.  Needless to say you will need a Arduino Rover (instructions not included) but you can look here [Arduino Rover](http://www.instructables.com/id/Little-Rover-ArduinoPicaxeTamiya-platform-las/) for suggestions.

nodeJS

node-serial

Arduino Uno [Arduino Uno](http://arduino.cc/en/Main/ArduinoBoardUno#.UxS-e3lBRqg)

Motor Shield [Motor Shield](https://www.sparkfun.com/products/9815)

Xbee Shield and Xbee pair [Xbee](https://www.sparkfun.com/categories/223)

_____
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

 A typical error results when you get the comm port wrong or the radio transceiver is not attached.

    $ node page.js 
	done init
	Server Running on 8084

	events.js:72
    throw er; // Unhandled 'error' event
              ^
	Error: Cannot open /dev/tty.usbserial-A900fwHn




___
### To Do

 - Ready the documentation, add tags to the code, add event handling and the science package to the code.
 - the README file itself has format issues so I need to learn how to format to my liking.
 - Better event handling for various error types from nodeJs and the Client side JavaScript.
 - Engine power updates to the robot. Since the Rover  treads are a bit quirky this allows the driver to adjust the power to the drive sprocket and just the course of the Rover. Otherwise the Rover  will drive with a curve.
 - Engine Impulse delay to the robot. The driver can adjust the length of time the command issued is executed on the Rover itself.
 - Create a Data structure that can pass easily between the robot and the client interface.
 - Better images for the read me. 
 - A flow diagram describing how the whole product works.
 - Add thermal sensors, arm and camera.
 





