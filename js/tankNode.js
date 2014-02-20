
$(function() {
	event.preventDefault();
        $("#forward").click(function(){
        	changeIndicator('#forwardImg', 'images/forward_green.jpg');
        	askForDataFromNode  ({color:"#ffffff",direction:"forward"});
        	askForDataFromNode  ({tankData:1});
	
        }); 
        $("#back").click(function(){
               changeIndicator('#backImg', 'images/back_green.jpg');
               askForDataFromNode  ({color:"#ffffff",direction:"back"});
               askForDataFromNode  ({tankData:1});
        });
        $("#left").click(function(){
        	changeIndicator('#leftImg', 'images/left_green.jpg');
        		askForDataFromNode  ({color:"#ffffff",direction:"left"});
               askForDataFromNode  ({tankData:1});
        });
        $("#right").click(function(){
        	changeIndicator('#rightImg', 'images/right_green.jpg');
        	askForDataFromNode ({color:"#ffffff",direction:"right"});
               askForDataFromNode  ({tankData:1});
        });
        $("#stop").click(function(){
		$("#status2").css("color", "#ff0000");
                changeIndicator("", "");
                askForDataFromNode ({color:"#ffffff",direction:"stop", stopevent:1});
		$("#status1").css("color", "#DE6262");
        });
	$("#slider-vertical2").slider({
      		orientation: "vertical",
      		range: "min",
      		min: 0,
      		max: 255,
      		value: 60,
      		slide: function( event, ui ) {
        		$( "#amount2" ).text( ui.value );
			askForDataFromNode ({engine:ui.value,engineUpDate:"left"});
      		},
		change: function( event, ui ) {
			console.log ("slider event");
		}
    	});


	$( "#slider-vertical2" ).on( "slidechange", function( event, ui ) {} ) ;


	$("#slider-vertical3").slider({
      		orientation: "vertical",
      		range: "min",
      		min: 0,
      		max: 255,
      		value: 60,
      		slide: function( event, ui ) {
        		$( "#amount3" ).text( ui.value );
			askForDataFromNode ({engine:ui.value,engineUpDate:"right"});
      		},
		change: function( event, ui ) {
			console.log ("slider event");
		}
    	});
	$( "#slider-vertical3" ).on( "slidechange", function( event, ui ) {} ) ;

});

var idsetPrev = "";
var locationImgPrev = "";

function changeIndicator (idset, locationImg) {
	if (idsetPrev) {
		$(idsetPrev).attr('src',locationImgPrev);
	}	
	idsetPrev = idset;
	locationImgPrev = $(idset).attr('src');
	$(idset).attr('src',locationImg);
}



var xmlhttp;
function loadXMLDoc(url,cfunc) {
	if (window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
  		xmlhttp=new XMLHttpRequest();
  	}
	else {   // code for IE6, IE5
  		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  	}
	xmlhttp.onreadystatechange=cfunc;
	xmlhttp.open("GET",url,true);
	xmlhttp.send();
}

function askForDataFromNode(data) {
	var str = $.param(data);
	//console.log ("string passed without tank data request is " + str);
	loadXMLDoc("index.html?" + str ,function() {
  		if (xmlhttp.readyState==4 && xmlhttp.status==200) {
			// we will want to set a data structure we get from the node control server 	
			//console.log ("response is " + xmlhttp.responseText );
			var feedback = JSON.parse( xmlhttp.responseText );
			$("#status1").text( feedback.range);
			$("#status2").text( feedback.lastCommand );
			$("#status3").text( feedback.alert);
			processFeedBack(feedback);
    		}
  	});
}

function processFeedBack (data) {
	var strJSON = JSON.stringify(data);
	console.log ("feedback test " + strJSON );
	if (data.alert) {
		$("#status3").css("color", "#FF6802");
		$("#status1").css("color", "#DE6262");
		$("#status2").css("color", "#DE6262");
		changeIndicator("","");
		return;
	}	
	else if (data.stopEvent == 1) {
		$("#status2").css("color", "#DE6262");
	}
	else {
		$("#status3").css("color", "#000000");
		$("#status1").css("color", "#000000");
		$("#status2").css("color", "#000000");
		console.log ("left engine " , data.leftEngine);
		$("#slider-vertical2").slider("value",data.leftEngine);
		$("#slider-vertical3").slider("value", data.rightEngine);
        	$( "#amount2" ).text(data.leftEngine );
        	$( "#amount3" ).text(data.rightEngine );
	}
	console.log ("-------  " + data.stopEvent + "---" + data.lastCommand);
}
