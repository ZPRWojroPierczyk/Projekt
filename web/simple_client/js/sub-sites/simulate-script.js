/************************** Google maps API **************************/
// Center of the map
var position = [52.22977, 21.01178];

// Cities and their coordinates
var cities =
{
    cities: {
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

/************ Map ************/
var latlng = new google.maps.LatLng(position[0], position[1]);
var myOptions = {
    zoom: 7,
    center: latlng,
    mapTypeId: google.maps.MapTypeId.ROADMAP
};
var map = new google.maps.Map(document.getElementById("mapCanvas"), myOptions);

/************ Markers ************/
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

function displayLorries() {
    var i = 1;
    for (x in cities.cities) {
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

/************ Directions ************/
var directionsService = new google.maps.DirectionsService();
var directionsRenderer = new google.maps.DirectionsRenderer();

directionsRenderer.setMap(map);

directionsService.route(
    {
        origin: new google.maps.LatLng(cities.cities.warszawa[0], cities.cities.warszawa[1]),
        destination: new google.maps.LatLng(cities.cities.poznan[0], cities.cities.poznan[1]),
        travelMode: 'DRIVING'
    },
    function (response, status) {
        if (status == 'OK') {
            directionsRenderer.setDirections(response);
        } else {
            window.alert('Directions request failed due to ' + status);
        }
    });

directionsRenderer.setMap(map);

/*********** Moving markers **********/
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

/******************** Simulation buttons ********************/

// Sending information about simulation mode (buttons state), and styling the active one
function sendInformation(element) {

    if(element.getAttribute("data-icon") == "pause"){
        clearInterval(server_querying);
    } else {
        server_querying = setInterval(querying, 10); // Could be done better
    }

    var xhttp;
    xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            mapUpdate(this.responseText);
        }
    };

    xhttp.open("POST", "", true);
    var headerName = "Information";
    var headerValue = "simulation-" + element.getAttribute("data-icon");
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send();

    styleActiveMode(element);
}

function styleActiveMode(element) {
    var i, icons;
    icons = document.getElementsByClassName("fas");
    for (i = 0; i < icons.length; i++) {
        icons[i].className = icons[i].className.replace(" active", "");
    }
    element.className += " active";
}

/************ Side menu ************/

function openSideMenu() {
    document.getElementById("mySideMenu").style.width = "250px";
}

function closeSideMenu() {
    document.getElementById("mySideMenu").style.width = "0";
}

/************ Side menu modification buttons ************/

// Could be done better
var buttonStates = [false, false,false, false, false];

function sideMenuButtonClick(element, number){
    
    for (var i = 0; i < buttonStates.length; i++){
        if(buttonStates[i] == true){
            if(i == number){
                buttonStates[i] = false;
                disableStyleButton(element);
                return;
            } else {
                buttonStates[i] = false;
                var diffElement = document.getElementsByClassName("sideMenuButton")[i];
                disableStyleButton(diffElement);
                buttonStates[number] = true;
                enableStyleButton(element);
                return;
            }
        }
    }

    buttonStates[number] = true;
    enableStyleButton(element);

    return;
}

function enableStyleButton(element){
    element.style.backgroundColor = "#4e5066";
    element.style.border = "3px solid white";
}

function disableStyleButton(element){
    element.style.backgroundColor = "#626588";
    element.style.border = "none";
}

function modificationsAccept(){
    //TODO
}

// Getting style property in string
/*function changeElementColor(element){
    var bc = window.getComputedStyle(element, null).getPropertyValue("background-color");
    if (bc == "rgb(98, 101, 136)") element.style.backgroundColor = "rgb(0, 0, 255)";
    else element.style.backgroundColor = "rgb(98, 101, 136)";
}*/

/******************** Constantly requesting for simulation snapshot ********************/

// Querying server for simulation snapshot data
var server_querying = setInterval(querying, 10); // every 10ms

function querying() {
    var xhttp;
    xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            mapUpdate(this.responseText);
        }
    };

    xhttp.open("POST", "", true);
    var headerName = "Information";
    var headerValue = "simulation-snapshot-request";
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send();
}

/******************** Updating map with simulation snapshot ********************/

function mapUpdate(jsonData) {
    jsonData = JSON.parse(jsonData);
    //TODO: updating map
}