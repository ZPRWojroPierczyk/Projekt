// --- * Uncomment when servers response ready
// *** * Comment when server response ready

/************************** Google maps API **************************/
// Center of the map
var position = [52.22977, 21.01178];

// Cities and their coordinates
var cities = {
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
// ***
var marker = new google.maps.Marker({
    position: latlng,
    map: map,
    title: "Latitude:" + position[0] + " | Longitude:" + position[1]
});
// ***
// ***
var lorryIcon = {
    anchor: new google.maps.Point(17.5, 17.5),
    scaledSize: new google.maps.Size(35, 35),
    url: '/imgs/lorry.png'
};
// ***
var agents = [];
var agentsProperties = [];
displayLorries();
// ***
// ***
function displayLorries() {
    var i = 0;
    for (x in cities.cities) {
        var lorryPosition = new google.maps.LatLng(cities.cities[x][0], cities.cities[x][1]);
        agents.push(new google.maps.Marker({
            position: lorryPosition,
            map: map,
            title: "Lorry from: " + x + " no." + i,
            icon: lorryIcon
        }));
        agents[agents.length - 1].setMap(map);
        agentsProperties[i] = {
            break: false,
            malfunction: false,
            accident: false
        }
        i++;
    }
}
// ***

/************ Directions ************/
// ***
var directionsService = new google.maps.DirectionsService();
var directionsRenderer = new google.maps.DirectionsRenderer();
// ***
directionsRenderer.setMap(map);
// ***
directionsService.route({
        origin: new google.maps.LatLng(cities.cities.warszawa[0], cities.cities.warszawa[1]),
        destination: new google.maps.LatLng(cities.cities.poznan[0], cities.cities.poznan[1]),
        travelMode: 'DRIVING'
    },
    function(response, status) {
        if (status == 'OK') {
            directionsRenderer.setDirections(response);
        } else {
            window.alert('Directions request failed due to ' + status);
        }
    }
);
// ***
/*********** Setting modification markers when click **********/

function mapClick(event) {
    //var result = [event.latLng.lat(), event.latLng.lng()];
    //transition(result);
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
        default:
    }
}

/*
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
}*/

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
        title: "Blokada x",
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
        title: "Korek ...",
        icon: congestionIcon
    }));

    congestions[congestions.length - 1].setMap(map);
}

/********************** Setting break **************************/

var breakIcon = {
    anchor: new google.maps.Point(25 + 17.5, 25 + 7.5),
    scaledSize: new google.maps.Size(20, 20),
    url: '/imgs/break.png'
};

//var radius = 0.056674;
var breaks = [];

function setBreak(lat, lng) {
    /*var current_distance = 10;
    var shortest_distance = 10;
    var agent_index = -1;
    var alat;
    var alng;
    agents.forEach(function (item, index, array) {
        alat = item.position.lat;
        alng = item.position.lng;
        current_distance = Math.sqrt((lat-alat)**2 + (lng-alng)**2);
        alert(alat);
        if(current_distance < shortest_distance){
            shortest_distance = current_distance;
            agent_index = index;
        }
    });
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
    }*/
    var breakPosition = new google.maps.LatLng(lat, lng);
    breaks.push(new google.maps.Marker({
        position: breakPosition,
        map: map,
        title: "Przerwa B)",
        icon: breakIcon
    }));
    breaks[breaks.length - 1].setMap(map);
}

/********************** Setting malfunction **************************/

var malfunctionIcon = {
    anchor: new google.maps.Point(0 + 17.5, 25 + 7.5),
    scaledSize: new google.maps.Size(20, 20),
    url: '/imgs/malfunction.png'
};

var malfunctions = [];

function setMalfunction(lat, lng) {
    var malfunctionPosition = new google.maps.LatLng(lat, lng);
    malfunctions.push(new google.maps.Marker({
        position: malfunctionPosition,
        map: map,
        title: "Awaria :(",
        icon: malfunctionIcon
    }));
    malfunctions[malfunctions.length - 1].setMap(map);
}

/********************** Setting accident **************************/

var accidentIcon = {
    anchor: new google.maps.Point(-25 + 17.5, 25 + 7.5),
    scaledSize: new google.maps.Size(20, 20),
    url: '/imgs/accident.png'
};

var accidents = [];

function setAccident(lat, lng) {
    var accidentPosition = new google.maps.LatLng(lat, lng);
    accidents.push(new google.maps.Marker({
        position: accidentPosition,
        map: map,
        title: "Wypadek [*]",
        icon: accidentIcon
    }));
    accidents[accidents.length - 1].setMap(map);
}

/******************** Simulation buttons ********************/

var pause_mode = false;

// Sending information about simulation mode (buttons state), and styling the active one
function sendInformation(element) {

    if (element.getAttribute("data-icon") == "pause") {
        // --- clearInterval(server_querying);
        google.maps.event.addListener(map, 'click', mapClick);
        pause_mode = true;
    } else {
        // --- server_querying = setInterval(querying, 10); // Could be done better
        google.maps.event.clearInstanceListeners(map, 'click');
        pause_mode = false;
        closeSideMenuLeft();
    }

    var xhttp;
    xhttp = new XMLHttpRequest();
    xhttp.open("POST", "", true);
    var headerName = "Information";
    var headerValue = "simulation/" + element.getAttribute("data-icon");
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

/************ Side menu Left ************/

function openSideMenuLeft() {
    if (pause_mode == true) {
        for (var i = 0; i < agents.length; i++) {
            google.maps.event.addListener(agents[i], 'click', agentClicked);
        }
        document.getElementById("mySideMenuLeft").style.width = "250px";
    } else alert("Modyfikacja tylko w trybie pauzy."); //can it block ajax and/or displaying?
}

function agentClicked(agent) {
    var i;
    for (i = 0; i < agents.length; i++) {
        if (agents[i].position.lat() == agent.latLng.lat() &&
            agents[i].position.lng() == agent.latLng.lng()) break;
    }
    if (buttonStates[2] == true && agentsProperties[i].break == false) {
        agentsProperties[i].break = true;
        setBreak(agent.latLng.lat(), agent.latLng.lng());
    } else if (buttonStates[3] == true && agentsProperties[i].malfunction == false) {
        agentsProperties[i].malfunction = true;
        setMalfunction(agent.latLng.lat(), agent.latLng.lng());
    } else if (buttonStates[4] == true && agentsProperties[i].accident == false) {
        agentsProperties[i].accident = true;
        setAccident(agent.latLng.lat(), agent.latLng.lng());
    }
}

function closeSideMenuLeft() {
    document.getElementById("mySideMenuLeft").style.width = "0";

    var len;

    len = blockades.length;
    for (var i = len - 1; i >= 0; i--) {
        blockades[i].setMap(null);
        delete blockades.pop();
    }

    len = congestions.length;
    for (var i = len - 1; i >= 0; i--) {
        congestions[i].setMap(null);
        delete congestions.pop();
    }

    len = breaks.length;
    for (var i = len - 1; i >= 0; i--) {
        breaks[i].setMap(null);
        delete breaks.pop();
    }

    len = malfunctions.length;
    for (var i = len - 1; i >= 0; i--) {
        malfunctions[i].setMap(null);
        delete malfunctions.pop();
    }

    len = accidents.length;
    for (var i = len - 1; i >= 0; i--) {
        accidents[i].setMap(null);
        delete accidents.pop();
    }

    for (var i = 0; i < buttonStates.length; i++) {
        buttonStates[i] = false;
    }
    var sideMenuButtons = document.getElementsByClassName("sideMenuButton");

    for (var i = 0; i < agents.length; i++) {
        google.maps.event.clearInstanceListeners(agents[i], 'click');
        agentsProperties[i].break = false;
        agentsProperties[i].malfunction = false;
        agentsProperties[i].accident = false;
    }

    for (var i = 0; i < sideMenuButtons.length; i++) disableStyleButton(sideMenuButtons[i]);
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
    var json_data = "{modifications:{";

    if (congestions.length != 0) {
        json_data += "\"congestions\":[";

        for (var i = 0; i < congestions.length; i++) {
            json_data += "[" + congestions[i].position.lat() + ", ";
            json_data += congestions[i].position.lng() + "],";
        }

        json_data = json_data.substr(0, json_data.length - 1);
        json_data += "],";
    }

    if (blockades.length != 0) {
        json_data += "\"blockades\":[";

        for (var i = 0; i < blockades.length; i++) {
            json_data += "[" + blockades[i].position.lat() + ", ";
            json_data += blockades[i].position.lng() + "],";
        }

        json_data = json_data.substr(0, json_data.length - 1);
        json_data += "],";
    }

    json_data += "\"agents\":[";

    for (var i = 0; i < agentsProperties.length; i++) {
        var no = agents[i].title.substring(agents[i].title.search("no.") + 3, agents[i].title.length);
        // --- var no = agents[i].title.substring(agents[i].title.search("ID") + 4, agents[i].title.search("Transport") - 1);
        json_data += "{\"ID\": " + no + ", ";
        json_data += "\"break\": " + agentsProperties[i].break+", ";
        json_data += "\"malfunction\": " + agentsProperties[i].malfunction + ", ";
        json_data += "\"accident\": " + agentsProperties[i].accident + "},";
    }

    json_data = json_data.substr(0, json_data.length - 1);
    json_data += "]}}";

    var xhttp;
    xhttp = new XMLHttpRequest();

    xhttp.open("POST", "", true);
    var headerName = "Content-Type";
    var headerValue = "simulation/modifications";
    var sendString = json_data;
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send(sendString);
}


/************ Side menu Right ************/

function openSideMenuRight() {
    document.getElementById("mySideMenuRight").style.width = "250px";
}

function closeSideMenuRight() {
    document.getElementById("mySideMenuRight").style.width = "0";
}

// Getting style property in string
/*function changeElementColor(element){
    var bc = window.getComputedStyle(element, null).getPropertyValue("background-color");
    if (bc == "rgb(98, 101, 136)") element.style.backgroundColor = "rgb(0, 0, 255)";
    else element.style.backgroundColor = "rgb(98, 101, 136)";
}*/

/******************** Constantly requesting for simulation snapshot ********************/

// Querying server for simulation snapshot data
// --- var server_querying = setInterval(querying, 10); // every 10ms

function querying() {
    var xhttp;
    xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            mapUpdate(this.responseText);
        }
    };

    xhttp.open("POST", "", true);
    var headerName = "Information";
    var headerValue = "simulation/snapshot-request";
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send();
}

/******************** Updating map with simulation snapshot ********************/

var lorryIcon = {
    anchor: new google.maps.Point(12, 12),
    scaledSize: new google.maps.Size(25, 25),
    url: '/imgs/lorry.png'
};

// --- var directionsRendererArray = [];
// --- var agents = [];
// --- var agentsProperties = [];

function mapUpdate(jsonData) {
    jsonData = JSON.parse(jsonData);
    clearMap();
    for (var i = 0; i < jsonData.transports.length; i++) {
        var transport = jsonData.transports[i];
        //Marker
        var agentLatlng = new google.maps.LatLng(transport.position[0], transport.position[1]); // Number()
        agents.push(new google.maps.Marker({
            position: agentLatlng,
            map: map,
            title: "Miasto przynal.: " + transport.city + "\nID: " + transport.ID +
                "\nTransport z: " + transport.from + " do: " + transport.to +
                "\nŁadunek: " + transport.load +
                "\nPozycja: " + transport.position[0] + ", " + transport.position[1] +
                "\nPrzerwa: " + transport.break+
                "\nAwaria: " + transport.malfunction +
                "\nWypadek: " + transport.accident,
            icon: lorryIcon
        }));
        agents[agents.length - 1].setMap(map);
        agentsProperties[i] = {
                break: false,
                malfunction: false,
                accident: false
            }
            //Direction
        directionsRendererArray[i] = new google.maps.DirectionsRenderer();
        directionsRendererArray[i].setMap(map);
        directionsRendererArray[i].setDirections(transport.path);
        //Transports content
        var transportsContent = "<p>Z " + transport.from + " do " + transport.to + ". Ładunek: ";
        transportsContent += transport.load + ". Status: " + transport.transportStatus + ".</p>";
        document.getElementById("transportsContent").innerHTML = transportsContent;
    }
}

function clearMap() {
    var len;

    len = agents.length;
    for (var i = len - 1; i >= 0; i--) {
        agents[i].setMap(null);
        delete agents.pop();
    }

    len = directionsRendererArray.length;
    for (var i = len - 1; i >= 0; i--) {
        directionsRendererArray[i].setMap(null);
        delete directionsRendererArray[i].pop();
    }
}