sys     = require("sys"),  
my_http = require("http");  
path    = require("path"),  
url     = require("url"),  
filesys = require("fs");  
eventsA = require("events");

function roverDataInit() {
	this.lastCommand = "starting";
	this.status = true;
	this.heart = false;
	this.range = 33;
	this.alert = "";
	this.roverMessage = "";
	this.roverData = "";
	this.leftEngine = 255;
	this.rightEngine = 255;
	this.engineImpulse = 100;
	this.radarData = "";

	this.setLastCommand=function(strValue){
     	this.lastCommand=strValue;
    };
	this.getLastCommand = function(){
     	return this.lastCommand;
    };    

    this.setRadarData=function(strValue){
    	//console.log("reading roverdata" + strValue);
     	this.radarData=strValue;
    };
	this.getRadarData = function(){
		//console.log("getting radar data");
     	return this.radarData;
    };    

	
	this.setHeartBeat = function  (strValue) {
		this.Status = signal;	
	};
	this.getHeartBeat = function() {
		return this.Status;
    }	

	this.setRange=function(strValue) {
		this.range = strValue;
	};
	this.getRange=function(strValue) {
     		return this.range;
	}

	this.setAlert=function(strValue) {
		this.alert = strValue;
	};	
	this.getAlert=function(strValue) {
     		return this.alert;
	}

	this.setRoverInfo=function(strValue) {
		this.roverMessage = strValue;
	};	
	this.getRoverInfo=function(strValue) {
     		return this.roverMessage;
	}

	this.setRoverData=function(strValue) {
		this.roverData = strValue;
	};	
	this.getRoverData=function(strValue) {
     		return this.roverData;
	
	}

	this.setEngineLeft=function(strValue) {
		this.leftEngine= strValue;
	};	
	this.getEngineLeft=function(strValue) {
     	return this.leftEngine;
	}

	this.setEngineRight=function(strValue) {
		this.rightEngine = strValue;
	};	
	this.getEngineRight=function(strValue) {
     	return this.rightEngine;
	}

	this.setEngineImpulse = function(strValue) {
		this.engineImpulse = strValue;
	};	
	this.getEngineImpulse = function(strValue) {
     		return this.engineImpulse;
	}
	console.log ("done init");			
	
}
var webPort = 8084;
var robotData = new roverDataInit();
var comPort = '/dev/tty.usbserial-A900fwHn';
var serialport = require("serialport");
var SerialPort = serialport.SerialPort; // localize object constructor

try { 
var sp = new SerialPort(comPort, {
  parser: serialport.parsers.readline(),
  baudrate: 9600,
  error: function( er, messsage) { 
  		console.log("error " + err + "\n" + "message" + message);ß
  }
});
}
catch(er) {
	console.log("error com port likely the XBEE is not attached");
}


sp.on("error", function (error) {
	console.log("error alex" + error);
});

sp.on("open", function () {
    sp.write(0x80);
    sp.write('123456\r');
    console.log ("comm port ready");
});

my_http.createServer(function(request,response) {  
 	// parsing paths requested helps give html response
    var my_path = url.parse(request.url).pathname;  
    var full_path = path.join(process.cwd(),my_path);  
	
    path.exists(full_path,function(exists) {  
       	if(!exists) {  
           		response.writeHeader(404, {"Content-Type": "text/plain"});    
           		response.write("404 Not Found\n");    
           		response.end();  
       	}  
       	else {             		
       		filesys.readFile(full_path,  function(err, file) {    
           		if(err) {    
                    response.writeHeader(500, {"Content-Type": "text/plain"});    
           			response.write("please select a project\n");    
                    response.write("The is the base directory for all.\n");    
                    response.end();    
                 }    
                 else {  
					var _get = url.parse(request.url, true).query; 

                    response.writeHeader(200);    
					
					if (_get.roverData  && !_get.direction) {
						//console.log ("rover data request for browser")
						var sendToBrowser = JSON.stringify(robotData);
						//console.log(sendToBrowser)
						response.write (sendToBrowser);
					}						
					else if (_get.engineUpDate) {
						//console.log("engineUpdate");
						if (_get.engineUpDate == "left") {
							robotData.setEngineLeft(_get.engine);
						}
						else if (_get.engineUpDate == "power"){
							robotData.setEngineImpulse(_get.engine);	
						}
						else {
							robotData.setEngineRight(_get.engine);
						}
						var sendToBrowser = JSON.stringify(robotData);
						response.write (sendToBrowser);
					}
					else if (!_get.stopevent  && _get.direction) {
						//console.log("direction request");
						robotData.setLastCommand(_get.direction);
						var sendToBrowser = JSON.stringify(robotData);
						response.write (sendToBrowser);
					}
					else if (_get.stopevent == 1) {
						//console.log("stop event");
						robotData.setLastCommand("stop")
						var sendToBrowser = JSON.stringify(robotData );
						response.write (sendToBrowser);
					}
					else {
						response.write(file);
					}
					response.end();  
                }  
                       
            });  
       	}  
    });  

 	sp.on('data', function (arduinoData) {

 		//console.log("\n\n\n----\ntest serial from Arduino \n------ \n\n\n")
		if (arduinoData.length < 2) {
			return;
		}
    	var ArduinoString = arduinoData.toString();
		var re = /(ALERT|Rover|data|radar)/i;
		var match = re.exec(ArduinoString);
		//console.log ("data recieved ->" + ArduinoString + "<- aaa ");
		//console.log ("data recieved ->" + arduinoData + "<- aaa ");
		
		if (match) {
			console.log ("data recieved ->" + ArduinoString + "<- aaa ");
			matchResults = match[0].toLowerCase();
			if (matchResults == "alert") {
				var splitResults = ArduinoString.split(",");
				//console.log("Alert  data " + splitResults.length +  "  " + data);
				robotData.setAlert(splitResults[1]);
				robotData.setRange(splitResults[2]);
			}
			else if (matchResults == "rover") {
				var splitResults = ArduinoString.split(",");
				//console.log("rover data " + splitResults.length +  "  " + ArduinoString);
				robotData.setRoverInfo(ArduinoString);
				robotData.setLastCommand(splitResults[1]);
				robotData.setRange(splitResults[5]);
				robotData.setEngineLeft(splitResults[2]);
				robotData.setEngineRight(splitResults[3]);
				robotData.setEngineImpulse(splitResults[4]);
				robotData.setAlert("");
			}
			else if (matchResults == "radar") {
				robotData.setRadarData(ArduinoString);
			}
			else if (matchResults == "data") {
				robotData.setRoverData(ArduinoString);
			}
			else {
				//console.log("->unmatched data ", match[0], data);
			}
		}
		match = "";

  	});
 	request.on('end', function () { 
 		//console.log ("------\nparsing the data\n___________\n");

		//var _get = url.parse(request.url); //, true).query; 
		var _get = url.parse(request.url, true).query; 
		//{ engine: '101', engineUpDate: 'left' }
	
		var sendToArduino = "";	
		if (_get.direction) {
			//console.log ("direction ---------- " + _get.direction);
			sendToArduino = new String (  _get.direction).toLowerCase() ;
			robotData.setLastCommand(sendToArduino);
				console.log(sendToArduino);
		}	
		else if (_get.engineUpDate) {
			console.log ("engineUpdate ->" + _get.engine + " " + _get.engineUpDate);
			var engineSend = "engine" + _get.engineUpDate  + ":" + _get.engine + "\r";
			sendToArduino = new String (engineSend).toLowerCase() ;
			console.log(sendToArduino);
		}
		else if (_get.roverData) {
			//console.log("roverData xxxxxxxxxxx");
			sendToArduino =  new String ("roverData").toLowerCase() + "\r";
		}
		else if (_get.radarData) {
			//console.log("radardata xxxxxxxxxxx");
			sendToArduino =  new String ("radardata").toLowerCase() + "\r";
			console.log(sendToArduino);
		}

		else {
			//console.log ("aaaa no data");
			return;
		}

		//console.log ("sending ... we hope.  ->> " + sendToArduino + "<--- ");
		sendToArduino = sendToArduino + "\r";
		sp.write(sendToArduino , function(err, results) {
			sp.drain(function(err, result){
				//console.log ("drain " + sendToArduino);
				//console.log(err, result);
			});
			//console.log ("results -> " + results);
		});
    }); 
}).listen(webPort);  
sys.puts("Server Running on " + webPort);           



