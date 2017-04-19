
var center =  {lat: -29.717867, lng: -53.717378};
var i;
var pointsRoute = [{lat: -29.710173, lng: -53.716594},
{lat: -29.705787, lng: -53.715961},
{lat: -29.705293, lng: -53.715718},
{lat: -29.705244, lng: -53.716063},
{lat: -29.705722, lng: -53.716054},
{lat: -29.709991, lng: -53.716707},
{lat: -29.710737, lng: -53.716693},
{lat: -29.711810, lng: -53.716515},
{lat: -29.712242, lng: -53.719246},
{lat: -29.715677, lng: -53.718637},
{lat: -29.715267, lng: -53.715848},
{lat: -29.716060, lng: -53.715677},
{lat: -29.719159, lng: -53.715061},
{lat: -29.719554, lng: -53.717932},
{lat: -29.721895, lng: -53.717508},
{lat: -29.722654, lng: -53.723044},
{lat: -29.729176, lng: -53.721712},
{lat: -29.728947, lng: -53.719800},
{lat: -29.730092, lng: -53.719642},
{lat: -29.728939, lng: -53.719791},
{lat: -29.728435, lng: -53.716317},
{lat: -29.725455, lng: -53.716864},
{lat: -29.725059, lng: -53.714266},
{lat: -29.724784, lng: -53.712402},
{lat: -29.724646, lng: -53.710987},
{lat: -29.722872, lng: -53.711337},
{lat: -29.720057, lng: -53.711882},
{lat: -29.720684, lng: -53.716483},
{lat: -29.719519, lng: -53.716707},
{lat: -29.719269, lng: -53.715010},
{lat: -29.716008, lng: -53.715652},
{lat: -29.715818, lng: -53.715619},
{lat: -29.712755, lng: -53.716135},
{lat: -29.710173, lng: -53.716594}];

var pointsStop = [{lat: -29.711392, lng: -53.716317},
									{lat: -29.709379, lng: -53.716468},
									{lat: -29.706658, lng: -53.716061},
									{lat: -29.706639, lng: -53.716211},
									{lat: -29.709398, lng: -53.716683}];

var iconMarker;
//var iconBus = 'static/img/bufsm.png';
var iconBus;


//Iniatialize google maps API and go to the IoT portal to update List of things
function initMap() {
	map = new google.maps.Map(document.getElementById('map'), {center: center, scrollwheel: false, zoom: 15});
  var busPath = new google.maps.Polyline({
    path: pointsRoute,
    geodesic: true,
    strokeColor: '#0000FF',
    strokeOpacity: 1.0,
    strokeWeight: 2
  });
	iconBus = {
	    url: "static/img/bufsm.png", // url
	    scaledSize: new google.maps.Size(25, 32), // scaled size
	    origin: new google.maps.Point(0,0), // origin
	    anchor: new google.maps.Point(0, 0) // anchor
	};
	iconMarker = {
	    url: 'static/img/marker.png', // url
	    scaledSize: new google.maps.Size(30, 35), // scaled size
	    origin: new google.maps.Point(0,0), // origin
	    anchor: new google.maps.Point(0, 0) // anchor
	};
	for(i = 0; i<5; i++){
  	addBusStop(pointsStop[i]);
	}
	addBus(pointsStop[0]);
  busPath.setMap(map);
  updatePosition();
}

function updatePosition(){
    i = 0;
    window.setInterval(function () {
      if(i<10){
        //map.setCenter(points[i]);
        i++;
      }
      else {
        i = 0;
      }

    }, 1000); // repeat forever, polling every 3 seconds
}
function addBusStop(location) {
        marker = new google.maps.Marker({
            position: location,
						icon: iconMarker,
            map: map
        });
}
function addBus(location) {
        marker = new google.maps.Marker({
            position: location,
						icon: iconBus,
            map: map
        });
}

// var map = L.map('map'),
//     trail = {
//         type: 'Feature',
//         properties: {
//             id: 1
//         },
//         geometry: {
//             type: 'LineString',
//             coordinates: []
//         }
//     },
//     realtime = L.realtime(function(success, error) {
//         L.Realtime.reqwest({
//             url: 'https://api.thingspeak.com/channels/250075/feeds.json?results=1',
//             crossOrigin: false,
//             type: 'json'
//         })
//         .then(function(data) {
//             var trailCoords = trail.geometry.coordinates;
//
//             coords = [parseFloat(data.feeds[0].field2), parseFloat(data.feeds[0].field1)];
//
//             trailCoords.push(coords);
//             trailCoords.splice(0, Math.max(0, trailCoords.length - 5));
//              success({
//                  type: 'FeatureCollection',
//                  features: [data, trail]
//              });
//         })
//         .catch(error);
//     }, {
//         interval: 4 * 1000
//     }).addTo(map);
//
//
//     L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
//         attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
//     }).addTo(map);
//
//     realtime.on('update', function() {
//         map.fitBounds(realtime.getBounds(), {maxZoom: 30});
//     });
