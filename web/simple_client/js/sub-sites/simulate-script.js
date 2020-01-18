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
var agents = [];
displayLorries();

function displayLorries() {
    var i = 1;
    for (x in cities.cities) {
        var lorryPosition = new google.maps.LatLng(cities.cities[x][0], cities.cities[x][1]);
        agents.push(new google.maps.Marker({
            position: lorryPosition,
            map: map,
            title: "Lorry no." + i + " " + x,
            icon: lorryIcon
        }));

        agents[agents.length-1].setMap(map);
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
    }
);

directionsRenderer.setMap(map);

/*********** Setting modification markers when click **********/

google.maps.event.addListener(map, 'click', function (event) {
    var result = [event.latLng.lat(), event.latLng.lng()];
    transition(result);
    var index = -1;
    for (var i = 0; i < buttonStates.length; i++) {
        if (buttonStates[i] == true) index = i;
    }
    switch (index) {
        case 0: // blockade
            setBlockade(event.latLng.lat(), event.latLng.lng());
            break;
        case 1: // congestion
            setCongestion(event.latLng.lat(), event.latLng.lng());
            break;
        case 2: // break
            setBreak(event.latLng.lat(), event.latLng.lng());
            break;
        case 3: // malfunction
            setMalfunction(event.latLng.lat(), event.latLng.lng());
            break;
        case 4: // accident
            setAccident(event.latLng.lat(), event.latLng.lng());
            break;
        default:
    }
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

/********************** Setting blockades **************************/

var blockadeIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/blockade.png'
};

var blockades = [];

function setBlockade(lat, lng) {
    var blockadePosition = new google.maps.LatLng(lat, lng);
    blockades.push(new google.maps.Marker({
        position: blockadePosition,
        map: map,
        title: "Just blockade ...",
        icon: blockadeIcon
    }));

    blockades[blockades.length - 1].setMap(map);
}

/********************** Setting congestions **************************/

var congestionIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/congestion.png'
};

var congestions = [];

function setCongestion(lat, lng) {
    var congestionPosition = new google.maps.LatLng(lat, lng);
    congestions.push(new google.maps.Marker({
        position: congestionPosition,
        map: map,
        title: "Just congestion ...",
        icon: congestionIcon
    }));

    congestions[congestions.length - 1].setMap(map);
}

/********************** Setting break **************************/

var breakIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/break.png'
};

var radius = 0.056674;
var breaks = [];

function setBreak(lat, lng) {
    var current_distance = 10;
    var shortest_distance = 10;
    var agent_index = -1;
    var alat;
    var alng;
    /*agents.forEach(function (item, index, array) {
        alat = item.position.lat;
        alng = item.position.lng;
        current_distance = Math.sqrt((lat-alat)**2 + (lng-alng)**2);
        alert(alat);
        if(current_distance < shortest_distance){
            shortest_distance = current_distance;
            agent_index = index;
        }
    });*/
    for(var i = 0; i < agents.length; i++){
        alat = agents[i].position.lat();
        alng = agents[i].position.lng();
        current_distance = Math.sqrt((lat-alat)**2 + (lng-alng)**2);
        if(current_distance < shortest_distance){
            shortest_distance = current_distance;
            agent_index = i;
        }
    }
    if(shortest_distance <= radius){
        //agents[agent_index] //oznaczyc ze ma przerwe -> tworzyc json do wyslania po klik zatwierdz
        var breakPosition = new google.maps.LatLng(agents[agent_index].position.lat(), agents[agent_index].position.lng());
        breaks.push(new google.maps.Marker({
            position: breakPosition,
            map: map,
            title: "Just congestion ...",
            icon: breakIcon
        }));
        breaks[breaks.length-1].setMap(map);
    }
}

/********************** Setting malfunction **************************/

var malfunctionIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/malfunction.png'
};

function setMalfunction(lat, lng) {

}

/********************** Setting accident **************************/

var accidentIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/accident.png'
};

function setAccident(lat, lng) {

}

/******************** Simulation buttons ********************/

// Sending information about simulation mode (buttons state), and styling the active one
function sendInformation(element) {

    if (element.getAttribute("data-icon") == "pause") {
        clearInterval(server_querying);
    } else {
        //server_querying = setInterval(querying, 10); // Could be done better
    }

    var xhttp;
    xhttp = new XMLHttpRequest();
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
    
    blockades.forEach(function (item, index, array) {
        item.setMap(null);
    });
    blockades = [];

    congestions.forEach(function (item, index, array) {
        item.setMap(null);
    });
    congestions = [];

    breaks.forEach(function (item, index, array) {
        item.setMap(null);
    });
    breaks = [];
}

/************ Side menu modification buttons ************/

// Could be done better
var buttonStates = [false, false, false, false, false];

function sideMenuButtonClick(element, number) {

    for (var i = 0; i < buttonStates.length; i++) {
        if (buttonStates[i] == true) {
            if (i == number) {
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

function enableStyleButton(element) {
    element.style.backgroundColor = "#4e5066";
    element.style.border = "3px solid white";
}

function disableStyleButton(element) {
    element.style.backgroundColor = "#626588";
    element.style.border = "none";
}

function modificationsAccept() {
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
//var server_querying = setInterval(querying, 10); // every 10ms

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

var lorryIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/lorry.png'
};

//var agents = [];

function mapUpdate(jsonData) {
    jsonData = JSON.parse(jsonData);
    var directionsRendererArray = [];
    for (var i = 0; i < jsonData.transports.length; i++) {
        var transport = jsonData.transports[i];
        //Marker
        var agentLatlng = new google.maps.LatLng(transport.position[0], transport.position[1]);
        agents.push(new google.maps.Marker({
            position: agentLatlng,
            map: map,
            title: "Home city: " + transport.city + "\nAgent ID: " + transport.ID +
                "\nTransport form: " + transport.from + " to: " + transport.to +
                "\nLoad: " + transport.load +
                "\nPosition: " + transport.position[0] + ", " + transport.position[1],
            icon: lorryIcon
        }));
        agents[agents.length-1].setMap(map);
        //Direction
        directionsRendererArray[i] = new google.maps.DirectionsRenderer();
        directionsRendererArray[i].setDirections(transport.path);
    }
}