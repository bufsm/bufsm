
var map = L.map('map'),
    trail = {
        type: 'Feature',
        properties: {
            id: 1
        },
        geometry: {
            type: 'LineString',
            coordinates: []
        }
    },
    realtime = L.realtime(function(success, error) {
        L.Realtime.reqwest({
            url: 'https://api.thingspeak.com/channels/250075/feeds.json?results=1',
            crossOrigin: false,
            type: 'json'
        })
        .then(function(data) {
            var trailCoords = trail.geometry.coordinates;

            coords = [parseFloat(data.feeds[0].field2), parseFloat(data.feeds[0].field1)];

            trailCoords.push(coords);
            trailCoords.splice(0, Math.max(0, trailCoords.length - 5));
             success({
                 type: 'FeatureCollection',
                 features: [data, trail]
             });
        })
        .catch(error);
    }, {
        interval: 4 * 1000
    }).addTo(map);


    L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
    }).addTo(map);

    realtime.on('update', function() {
        map.fitBounds(realtime.getBounds(), {maxZoom: 30});
    });
