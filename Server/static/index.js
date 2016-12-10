var map = L.map('map'),
    realtime = L.realtime({
        url: 'http://bufsm.dalmago.xyz/api/list',
        crossOrigin: true,
        type: 'json'
    }, {
        interval: 4 * 1000
    }).addTo(map);

L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

realtime.on('update', function() {
    map.fitBounds(realtime.getBounds(), {maxZoom: 30});
});
