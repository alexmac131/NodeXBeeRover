NodeTank X-1
==
### What is it?
A combination of jQuery, nodeJs, Xbee and Arduino. This drives a custom built robot through web page interface around its environment.  This is still experimental and code changes often.
_____
## The tank

![alt tag](/images/tank.JPG)

#### Radio transceiver.
![alt tag](/images/xBee.JPG)


## The Interface
![alt tag](/images/controls.jpg)


______
### Required 
nodeJS
node-serial

Arduino Uno
Motor Shield
Xbee Shield and Xbee pair
Other hardware for a tank

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
 - Engine power updates to the robot. Since the tank treads are a bit quirky this allows the driver to adjust the power to the drive sprocket and just the course of the tank. Otherwise the tank will drive with a curve.
 - Engine Impulse delay to the robot. The driver can adjust the length of time the command issued is executed on the tank itself.
 - Create a Data structure that can pass easily between the robot and the client interface.
 - Better images for the read me. 
 - A flow diagram describing how the whole product works.
 - An X-2 vehicle that is more stable hardware wise as the whole X-1 is really "interesting" to manage; not, so something wheeled.
 - Add a sensor module attachment for servos. arm and camera.
 





