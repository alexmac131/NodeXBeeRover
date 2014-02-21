sys     = require("sys"),  
my_http = require("http");  
path    = require("path"),  
url     = require("url"),  
filesys = require("fs");  
eventsA = require("events");

function tankDataInit() {
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

	this.setLastCommand=function(strValue){
     	this.lastCommand=strValue;
    };
	this.getLastCommand = function(){
     	return this.lastCommand;
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
var robotData = new tankDataInit();
var comPort = '/dev/tty.usbserial-A900fwHn';
var serialport = require("serialport");
var SerialPort = serialport.SerialPort; // localize object constructor

var sp = new SerialPort(comPort, {
  parser: serialport.parsers.readline("\r"),
  baudrate: 9600
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
           			response.write(err + "\n");    
                    response.write(full_path + "\n");    
                    response.write(file + "\n");    
                    response.end();    
                 }    
                 else {  
					var _get = url.parse(request.url, true).query; 

                    response.writeHeader(200);    
					
					if (_get.tankData  && !_get.direction) {
						var sendToBrowser = JSON.stringify(robotData);
						response.write (sendToBrowser);
					}						
					else if (_get.engineUpDate) {
						if (_get.engineUpDate == "left") {
							robotData.setEngineLeft(_get.engine);
						}
						else {
							robotData.setEngineRight(_get.engine);
						}
						var sendToBrowser = JSON.stringify(robotData);
						response.write (sendToBrowser);
					}
					else if (!_get.stopevent  && _get.direction) {
						robotData.setLastCommand(_get.direction);
						var sendToBrowser = JSON.stringify(robotData);
						response.write (sendToBrowser);
					}
					else if (_get.stopevent == 1) {
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

 	sp.on('data', function (data) {
		if (data.length < 1) {
			return;
		}
    	data = data.toString();
		var re = /(ALERT|Rover|data)/i;
		match = re.exec(data);
		
		if (match) {
			matchResults = match[0].toLowerCase();
			if (matchResults == "alert") {
				var splitResults = data.split(",");
				//console.log("Alert  data " + splitResults.length +  "  " + data);
				robotData.setAlert(splitResults[1]);
				robotData.setRange(splitResults[2]);
			}
			else if (matchResults == "rover") {
				var splitResults = data.split(",");
				//console.log("rover data " + splitResults.length +  "  " + data);
				robotData.setRoverInfo(data);
				robotData.setLastCommand(splitResults[1]);
				robotData.setRange(splitResults[5]);
				robotData.setEngineLeft(splitResults[2]);
				robotData.setEngineRight(splitResults[3]);
				robotData.setEngineImpulse(splitResults[4]);
				robotData.setAlert("");
			}
			else if (matchResults == "data") {
				robotData.setRoverData(data);
			}
			else {
				//console.log("->unmatched data ", match[0], data);
			}
		}

  	});
 	request.on('end', function () { 
		var _get = url.parse(request.url, true).query; 
		var direction = new String (  _get.direction).toLowerCase();
		var sendFlag =  false;
		if (direction == "left") {
			sendFlag = true;
		}
		else if (direction == "right") {
			sendFlag = true;
		}
		else if (direction == "forward") {
			sendFlag = true;
		}
		else if (direction == "back") {
			sendFlag = true;
		}
		/* else if (direction == "stop") {
			sendFlag = true;
 		}  do not need this event as we are not on continous drive */
		else {


		}
		if (sendFlag) {
			//console.log ("sending ... we hope ..." + direction);
			robotData.setLastCommand(direction);
			sp.write(direction , function(err, results) {
				sp.drain(function(err, result){
					//console.log ("drain");
    					//console.log(err, result);
				});
				//console.log ("results -> " + results);
  			});
		}
    }); 
}).listen(8084);  
sys.puts("Server Running on 8084");           



