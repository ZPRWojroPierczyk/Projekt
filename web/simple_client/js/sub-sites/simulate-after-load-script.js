/******************** Communication with server ********************/

// Querying server for simulation snapshot data
//var server_querying = setInterval(querying, 5000);

function querying() {
    var json_snapshot_request = jsonSnapshotRequest();
    /*var xhttp;
    xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            mapUpdate(this.responseText);
            document.getElementById("myRange").value = Number(value) + 1;
        }
    };

    xhttp.open("POST", "snapshot_request.php", true);
    var headerName = "Content-type";
    var headerValue = "simulate/snapshot";
    var sendString = json_snapshot_request;
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send(sendString);*/
}

function jsonSnapshotRequest() {
    var jsonData = "{\"snapshotNumber\": ";
    jsonData += document.getElementById("myRange").value;
    jsonData += "}";
    alert(jsonData);
    return jsonData;
}

function mapUpdate(jsonData) {
    jsonData = JSON.parse(jsonData);
    //TODO: updating map
}

function sliderChanged() {
    var snapshot_number = document.getElementById("myRange").value;
    document.getElementById("output").innerHTML = snapshot_number;
    /*var xhttp;
    xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            mapUpdate(this.responseText);
        }
    };

    xhttp.open("POST", "snapshot_request.php", true);
    var headerName = "Content-type";
    var headerValue = "application/x-www-snapshot-request";
    var sendString = json_snapshot_request;
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send(sendString);*/
}

function stopSimulation() {
    clearInterval(server_querying);
}

function resumeSimulation() {
    server_querying = setInterval(querying, 5000);
}

// Request with Response communication and service
//Example of use in HTML
//<button type="button" onclick="loadDoc('ajax_info.txt', myFunction)">Change Content</button>
/*
function loadDoc(url, cFunction) {
    var xhttp;
    xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            cFunction(this);
        }
    };

    xhttp.open("POST", url, true);
    var headerName = "Content-type";
    var headerValue = "application/x-www-form-urlencoded";
    var sendString = "fname=Henry&lname=Ford";
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send(sendString);
}

function myFunction1(xhttp) {
    var textFromServer = xhttp.responseText;
    var data = JSON.parse(textFromServer);
}
*/

/************************** GOOGLE MAPS API **************************/
//Center of the map
var position = [52.22977, 21.01178];

// Cities and their coordinates
var cities = 
{
    cities:{
        bialystok: [53.133610, 23.163585],
        bydgoszcz: [53.123606, 18.013118],
        gdansk: [54.351403, 18.644016],
        gdynia: [54.518952, 18.536605],
        katowice: [50.264683, 19.023546],
        kielce: [50.865505, 20.627999],
        krakow: [50.063938, 19.945820],
        lublin: [51.245820, 22.570292],
        lodz: [51.759499, 19.458618],
        olsztyn: [53.778019, 20.480117],
        poznan: [52.405892, 16.929126],
        rzeszow: [50.040481, 21.999096],
        szczecin: [53.427978, 14.551147],
        warszawa: [52.227808, 21.004883],
        wroclaw: [51.105505, 17.036494]
    }
}

/************ MAP ************/
var latlng = new google.maps.LatLng(position[0], position[1]);
var myOptions = {
    zoom: 7,
    center: latlng,
    mapTypeId: google.maps.MapTypeId.ROADMAP
};
var map = new google.maps.Map(document.getElementById("mapCanvas"), myOptions);

/************ MARKERS ************/
var marker = new google.maps.Marker({
    position: latlng,
    map: map,
    title: "Latitude:" + position[0] + " | Longitude:" + position[1]
});

var lorryIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/lorry.png'
};

displayLorries();

function displayLorries(){
    var i = 1;
    for (x in cities.cities){
        var lorryPosition = new google.maps.LatLng(cities.cities[x][0], cities.cities[x][1]);
        agent = new google.maps.Marker({
            position: lorryPosition,
            map: map,
            title: "Lorry no." + i + " " + x,
            icon: lorryIcon
        });

        agent.setMap(map);
        i++;
    }
    
}

/************ DIRECTIONS ************/
var directionsService = new google.maps.DirectionsService();
var directionsRenderer = new google.maps.DirectionsRenderer();

directionsRenderer.setMap(map);

directionsService.route(
    {
      origin: new google.maps.LatLng(cities.cities.warszawa[0],cities.cities.warszawa[1]),
      destination: new google.maps.LatLng(cities.cities.poznan[0],cities.cities.poznan[1]),
      travelMode: 'DRIVING'
    },
    function(response, status) {
      if (status == 'OK') {
        directionsRenderer.setDirections(response);
      } else {
        window.alert('Directions request failed due to ' + status);
      }
});

directionsRenderer.setMap(map);

/*********** MOVING MARKERS **********/
google.maps.event.addListener(map, 'click', function (event) {
    var result = [event.latLng.lat(), event.latLng.lng()];
    transition(result);
});

var numDeltas = 100;
var delay = 10; //milliseconds
var i = 0;
var deltaLat;
var deltaLng;

function transition(result) {
    i = 0;
    deltaLat = (result[0] - position[0]) / numDeltas;
    deltaLng = (result[1] - position[1]) / numDeltas;
    moveMarker();
}

function moveMarker() {
    position[0] += deltaLat;
    position[1] += deltaLng;
    var latlng = new google.maps.LatLng(position[0], position[1]);
    marker.setTitle("Latitude:" + position[0] + " | Longitude:" + position[1]);
    marker.setPosition(latlng);
    if (i != numDeltas) {
        i++;
        setTimeout(moveMarker, delay);
    }
}
/*
function createCars(agents){
    var i = 0;
    for ( ; i < agents.length; i++) {
        agents[i].latitude;
        agents[i].longitude;
        var marker = new google.maps.Marker({
            position: myCenter,
            icon:'/imgs/lorry.png'
        });
        marker.setMap(map);
    }
}

function createMarker(map){
    var latlng = new google.maps.LatLng(position[0], position[1]);
    var marker = new google.maps.Marker({
        position: latlng,
        map = map,
        icon:'/imgs/lorry.png'
    });
    marker.setMap(map);
}*/