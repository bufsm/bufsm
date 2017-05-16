var leaveTime = ["07:30", "08:00", "08:30", "09:00",
  "09:30", "10:00", "11:00", "11:30", "12:30", "13:00",
  "13:30", "14:00", "15:00", "16:00", "16:30",
  "17:00", "17:30", "18:30", "19:30", "20:30", "21:30", "22:00"
];

var bufsmCurrentLocation = {
  lat: null,
  lng: null
};
var userCurrentLocation = {
  lat: null,
  lng: null
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

var isMobile = false;
var map;
var user;
var busMarker;
const urlAPI = "https://bufsm-dalmago.rhcloud.com/linha/1";
var actualIndex = -1;
var error = false;

var firstTime = true;

//Set the MQTT client
client = new Paho.MQTT.Client('iot.eclipse.org', 443, "/ws", '');

client.connect({
  useSSL: true,
  onSuccess: function() {
    client.subscribe("bufsm");

    //If the user is admin
    var admin = location.search.split('admin=')[1]

    // Try HTML5 geolocation.
    if (navigator.geolocation) {
      navigator.geolocation.watchPosition(function(position) {
        var pos = {
          lat: position.coords.latitude,
          lng: position.coords.longitude
        };

        user.setPosition(pos);

        //Send the new position to the broker
        if (admin == 'true') {
          message = new Paho.MQTT.Message('{"lat": ' + pos.lat + ', "lng": ' + pos.lng + '}');
          message.destinationName = "bufsm";
          message.retained = true;
          client.send(message);
        }

      }, function() {
        error = true;
        toastr.error('<strong>Habilite sua Localização (GPS)</strong>');
      });
    } else {
      error = true;
      toastr.error('<strong>Habilite sua Localização (GPS)</strong>');
    }

  }
});

//When the connection is lost
client.onConnectionLost = function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
};

//When the message is received
client.onMessageArrived = function onMessageArrived(message) {

  //Parse JSON
  data = JSON.parse(message.payloadString)

  //Get the position
  bufsmCurrentLocation.lat = data.lat;
  bufsmCurrentLocation.lng = data.lng;

  if (firstTime) {
    map.setCenter(bufsmCurrentLocation);
    firstTime = false;
  }
  busMarker.setPosition(bufsmCurrentLocation);
};

$(document).ready(function() {

  //Check if the device is mobile
  if (/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|ipad|iris|kindle|Android|Silk|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(navigator.userAgent) ||
    /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(navigator.userAgent.substr(0, 4))) isMobile = true;

  //Config the notification
  toastr.options.timeOut = 0;
  toastr.options.extendedTimeOut = 0;
  toastr.options.closeButton = true;

  if (isMobile) {
    toastr.options.positionClass = 'toast-top-full-width';
  }

  //Event for the bus-finding button
  $('#findBus').click(function(e) {
    e.preventDefault();
    map.setCenter(bufsmCurrentLocation);
    map.setZoom(17);
  });

  //Populate the modal with all the departure times
  $(leaveTime).each(function(i, e) {
    $('.modal-body ul').append('<li>' + e + '</li>');
  });

  var toggle = false;

  //Hide and show the menubar when clicked
  $('.cd-logo').click(function() {

    $("#hide").animate({
      width: 'toggle'
    }, 350);

    $("#show").animate({
      width: 'toggle'
    }, 350);

    if (!toggle) {

      $('#show div[data-toggle="tooltip"]').tooltip('toggle');
      window.setTimeout(function() {
        $('#show div[data-toggle="tooltip"]').tooltip('toggle');
      }, 3000);

      toggle = true;
    }

  });

  //Update the bus departure based on the server datetime
  window.setInterval(function() {
    updateDeparture(new Date().toLocaleString('pt-BR'));
  }, 120*1000);

  //Show the tooltip for the menubar and hide it after 3s
  $('#hide div[data-toggle="tooltip"]').tooltip('toggle');
  window.setTimeout(function() {
    $('#hide div[data-toggle="tooltip"]').tooltip('toggle');
  }, 3000);

  //move nav element position according to window width
  moveNavigation();
  $(window).on('resize', function() {
    (!window.requestAnimationFrame) ? setTimeout(moveNavigation, 300): window.requestAnimationFrame(moveNavigation);
  });

  //mobile version - open/close navigation
  $('.cd-nav-trigger').on('click', function(event) {
    event.preventDefault();

    toastr.remove()

    if ($('header').hasClass('nav-is-visible')) $('.moves-out').removeClass('moves-out');

    $('header').toggleClass('nav-is-visible');
    $('.cd-main-nav').toggleClass('nav-is-visible');
    $('.cd-main-content').toggleClass('nav-is-visible');
  });

  //mobile version - go back to main navigation
  $('.go-back').on('click', function(event) {
    event.preventDefault();
    $('.cd-main-nav').removeClass('moves-out');
  });

  //open sub-navigation
  $('.cd-subnav-trigger').on('click', function(event) {
    event.preventDefault();
    $('.cd-main-nav').toggleClass('moves-out');
  });

  function moveNavigation() {
    var navigation = $('.cd-main-nav-wrapper');
    var screenSize = checkWindowWidth();
    if (screenSize) {
      //desktop screen - insert navigation inside header element
      navigation.detach();
      navigation.insertBefore('.cd-nav-trigger');
    } else {
      //mobile screen - insert navigation after .cd-main-content element
      navigation.detach();
      navigation.insertAfter('.cd-main-content');
    }
  }

  function checkWindowWidth() {
    var mq = window.getComputedStyle(document.querySelector('header'), '::before').getPropertyValue('content').replace(/"/g, '').replace(/'/g, "");
    return (mq == 'mobile') ? false : true;
  }

});

//Iniatialize google maps API and go to the IoT portal to update List of things
function initMap() {

  //Create the map
  map = new google.maps.Map(document.getElementById('map'), {
    fullscreenControl: true,
    gestureHandling: 'greedy',
    scrollwheel: false,
    zoom: 17,
    streetViewControl: false
  });

  //Add the bus to the map
  busMarker = new google.maps.Marker({
    icon: {
      url: "static/img/bufsm2.png", // url
      scaledSize: new google.maps.Size(25, 32), // scaled size
      origin: new google.maps.Point(0, 0), // origin
      anchor: new google.maps.Point(0, 0) // anchor
    },
    map: map
  });

  //Add the bus path to the map
  new google.maps.Polyline({
    path: pointsRoute,
    geodesic: true,
    strokeColor: '#0000FF',
    strokeOpacity: 1.0,
    strokeWeight: 2,
    map: map
  });

  //For each stop
  for (i = 0; i < pointsStop.length; i++) {

    //Add a new stop to the map
    new google.maps.Marker({
      position: pointsStop[i],
      icon: {
        url: 'static/img/marker.png', // url
        scaledSize: new google.maps.Size(30, 35), // scaled size
        origin: new google.maps.Point(0, 0), // origin
        anchor: new google.maps.Point(0, 0) // anchor
      },
      map: map
    });
  }

  //Add the user to the map
  user = new google.maps.Marker({
    icon: {
      url: "static/img/user.png", // url
      scaledSize: new google.maps.Size(32, 32), // scaled size
    },
    map: map
  });

}

function updateDeparture(timeStamp) {

  //Define the current hour, changing the text to a numeric value
  var currentHour = parseInt(timeStamp.slice(11, 13)) * 1000 + parseInt(timeStamp.slice(14, 16));

  var index = 0;
  var i = 0;

  //For each departure time in the array
  for (i = 0; i < leaveTime.length - 1; i++) {

    //Get the current departure
    var leaveHour = parseInt(leaveTime[i].slice(0, 2)) * 1000 + parseInt(leaveTime[i].slice(3, 5));

    //Get the next departure
    var leaveHourNext = parseInt(leaveTime[i + 1].slice(0, 2)) * 1000 + parseInt(leaveTime[i + 1].slice(3, 5));

    //When it finds, update the index to set new position in the array
    if (currentHour < leaveHourNext && currentHour >= leaveHour) {
      index = i + 1;
      break;
    } else {
      index = i + 2;
    }
  }

  //If the index has changed
  if (index !== actualIndex) {

    //Update the index
    actualIndex = index;

    //Update the notification system
    if (!error) {
      toastr.clear();
      error = false;
    }
    toastr.success('Próximo Ônibus sai às <strong>' + leaveTime[(index % leaveTime.length)] + '</strong>');

    //Update the modal information, setting the new departure time
    $('.modal-body ul li').siblings().removeClass("busActive").find('span').remove();
    $('.modal-body ul li:nth-child(' + ((index % leaveTime.length) + 1) + ')').addClass("busActive").append('<span style="font-weight:bold;"> - Próxima Saída </span>')

  }

}
