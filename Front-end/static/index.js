var leaveTime = ["01:00","01:10","01:20", "01:58","07:30", "08:00", "08:30", "09:00",
    "09:30", "10:00", "11:00", "11:30", "12:30", "13:00",
    "13:30", "14:00", "15:00", "16:00", "16:30",
    "17:00", "17:30", "18:30", "19:30", "20:30", "21:30", "22:00"
];

var center = {
    lat: -29.710173,
    lng: -53.716594
};
var bufsmCurrentLocation = {
    lat: -29.710173,
    lng: -53.716594
};
var i;
var pointsRoute = [{
        lat: -29.710173,
        lng: -53.716594
    },
    {
        lat: -29.705787,
        lng: -53.715961
    },
    {
        lat: -29.705293,
        lng: -53.715718
    },
    {
        lat: -29.705244,
        lng: -53.716063
    },
    {
        lat: -29.705722,
        lng: -53.716054
    },
    {
        lat: -29.709991,
        lng: -53.716707
    },
    {
        lat: -29.710737,
        lng: -53.716693
    },
    {
        lat: -29.711810,
        lng: -53.716515
    },
    {
        lat: -29.712242,
        lng: -53.719246
    },
    {
        lat: -29.715677,
        lng: -53.718637
    },
    {
        lat: -29.715267,
        lng: -53.715848
    },
    {
        lat: -29.716060,
        lng: -53.715677
    },
    {
        lat: -29.719159,
        lng: -53.715061
    },
    {
        lat: -29.719554,
        lng: -53.717932
    },
    {
        lat: -29.721895,
        lng: -53.717508
    },
    {
        lat: -29.722654,
        lng: -53.723044
    },
    {
        lat: -29.729176,
        lng: -53.721712
    },
    {
        lat: -29.728947,
        lng: -53.719800
    },
    {
        lat: -29.730092,
        lng: -53.719642
    },
    {
        lat: -29.728939,
        lng: -53.719791
    },
    {
        lat: -29.728435,
        lng: -53.716317
    },
    {
        lat: -29.725455,
        lng: -53.716864
    },
    {
        lat: -29.725059,
        lng: -53.714266
    },
    {
        lat: -29.724784,
        lng: -53.712402
    },
    {
        lat: -29.724646,
        lng: -53.710987
    },
    {
        lat: -29.722872,
        lng: -53.711337
    },
    {
        lat: -29.720057,
        lng: -53.711882
    },
    {
        lat: -29.720684,
        lng: -53.716483
    },
    {
        lat: -29.719519,
        lng: -53.716707
    },
    {
        lat: -29.719269,
        lng: -53.715010
    },
    {
        lat: -29.716008,
        lng: -53.715652
    },
    {
        lat: -29.715818,
        lng: -53.715619
    },
    {
        lat: -29.712755,
        lng: -53.716135
    },
    {
        lat: -29.710173,
        lng: -53.716594
    }
];

var pointsStop = [{
        lat: -29.711392,
        lng: -53.716317
    },
    {
        lat: -29.709379,
        lng: -53.716468
    },
    {
        lat: -29.706658,
        lng: -53.716061
    },
    {
        lat: -29.706639,
        lng: -53.716211
    },
    {
        lat: -29.709398,
        lng: -53.716683
    }
];

var iconMarker;
var iconBus;
var busMarker;
var urlAPI = "http://bufsm-dalmago.rhcloud.com/linha/1";

var actualIndex = -1;

//Iniatialize google maps API and go to the IoT portal to update List of things
function initMap() {
    map = new google.maps.Map(document.getElementById('map'), {
        center: center,
        fullscreenControl: true,
        gestureHandling: 'greedy',
        scrollwheel: false,
        zoom: 17,
        streetViewControl: false
    });
    var centerControlDiv = document.createElement('div');
    var centerControl = new CenterControl(centerControlDiv, map);

    centerControlDiv.index = 1;
    map.controls[google.maps.ControlPosition.LEFT_BOTTOM].push(centerControlDiv);

    var busPath = new google.maps.Polyline({
        path: pointsRoute,
        geodesic: true,
        strokeColor: '#0000FF',
        strokeOpacity: 1.0,
        strokeWeight: 2
    });
    iconBus = {
        url: "static/img/bufsm2.png", // url
        scaledSize: new google.maps.Size(25, 32), // scaled size
        origin: new google.maps.Point(0, 0), // origin
        anchor: new google.maps.Point(0, 0) // anchor
    };
    iconMarker = {
        url: 'static/img/marker.png', // url
        scaledSize: new google.maps.Size(30, 35), // scaled size
        origin: new google.maps.Point(0, 0), // origin
        anchor: new google.maps.Point(0, 0) // anchor
    };
    for (i = 0; i < pointsStop.length; i++) {
        addBusStop(pointsStop[i]);
    }
    addBus(bufsmCurrentLocation);
    busPath.setMap(map);
    updatePosition();
}

function updatePosition() {
    i = 0;
    window.setInterval(function() {
        var data;
        $.get(urlAPI, function(data, status) {
            bufsmCurrentLocation.lat = data.lat;
            bufsmCurrentLocation.lng = data.lng;
            console.log(center);
        });
        busMarker.setPosition(bufsmCurrentLocation);
    }, 1000); // repeat forever, polling every 1 seconds
}

function addBusStop(location) {
    marker = new google.maps.Marker({
        position: location,
        icon: iconMarker,
        map: map
    });
}

function addBus(location) {
    busMarker = new google.maps.Marker({
        position: location,
        icon: iconBus,
        map: map
    });
}

function CenterControl(controlDiv, map) {

    // Set CSS for the control border.
    var controlUI = document.createElement('div');
    controlUI.style.backgroundColor = '#fff';
    controlUI.style.border = '2px solid #fff';
    controlUI.style.borderRadius = '3px';
    controlUI.style.boxShadow = '0 2px 6px rgba(0,0,0,.3)';
    controlUI.style.cursor = 'pointer';
    controlUI.style.marginBottom = '22px';
    controlUI.style.textAlign = 'center';
    controlUI.title = 'Click to recenter the map';
    controlDiv.appendChild(controlUI);

    // Set CSS for the control interior.
    var controlText = document.createElement('div');
    controlText.style.color = 'rgb(25,25,25)';
    controlText.style.fontFamily = 'Roboto,Arial,sans-serif';
    controlText.style.fontSize = '16px';
    controlText.style.lineHeight = '38px';
    controlText.style.paddingLeft = '5px';
    controlText.style.paddingRight = '5px';
    controlText.innerHTML = 'BUFSM, cadê tu?';
    controlUI.appendChild(controlText);

    // Setup the click event listeners: simply set the map to Chicago.
    controlUI.addEventListener('click', function() {
        map.setCenter(bufsmCurrentLocation);
    });

}
$(document).ready(function() {
    $(leaveTime).each(function(i, e) {
        $('.modal-body ul').append('<li>' + e + '</li>');
    });
		updateDateSchedule();
		window.setInterval(function() {
				updateDateSchedule();
		}, 5000); // repeat forever, polling every 1 seconds

});

function updateDateSchedule(){
	var index = 0;
	var time;
	var urlDate = "http://api.geonames.org/timezoneJSON?formatted=true&lat=-29.710173&lng=-53.716594&username=demo&style=full";
	$.get(urlDate, function(data, status) {
			time = data.time;
			var i = 0;
			for (i = 0; i < leaveTime.length-1; i++) {
					var currentHour = parseInt(time.slice(11,13))*1000+parseInt(time.slice(14,16));
					var leaveHour = parseInt(leaveTime[i].slice(0, 2))*1000 + parseInt(leaveTime[i].slice(3, 5));
					var leaveHourNext = parseInt(leaveTime[i+1].slice(0, 2))*1000 + parseInt(leaveTime[i+1].slice(3, 5));
					if( currentHour < leaveHourNext && currentHour >= leaveHour ){
						index = i+1;
						break;
					}
					else {
						index = i+2;
					}
			}
			if(index !== actualIndex){
				actualIndex = index;
				$('.modal-body ul li').siblings().find('span').remove();
				$('.modal-body ul li').siblings().removeClass("busActive");

				$('.modal-body ul li:nth-child(' + ((index%leaveTime.length)+1)+ ')').append('<span style="font-weight:bold;color:#880000"> - Próxima Saída </span>')
				$('.modal-body ul li:nth-child(' + ((index %leaveTime.length)+1)+ ')').addClass("busActive");
			}
	});
}
