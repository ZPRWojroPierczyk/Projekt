/****************** Switching tabs *******************/

function openTab(evt, tab) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("inner-body");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tab).style.display = "block";
    evt.currentTarget.className += " active";
}

/****************** Cities form *******************/

var cities_valid = false;

function citiesAccept() {
    var x = document.getElementById("Cities-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }
    document.getElementsByClassName("accept")[0].style.display = "none";
    document.getElementsByClassName("go-back")[0].style.display = "inline";

    //TODO: Check if form is all right then flag cities_valid is true

    cities_valid = true;
}

function citiesGoBack() {
    var x = document.getElementById("Cities-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[0].style.display = "inline";
    document.getElementsByClassName("go-back")[0].style.display = "none";
    cities_valid = false;
}

/****************** Agent form *******************/

var agent_valid = false;

function agentAccept() {
    var x = document.getElementById("Agent-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }
    document.getElementsByClassName("accept")[1].style.display = "none";
    document.getElementsByClassName("go-back")[1].style.display = "inline";

    //TODO: Check if form is all right then flag agent_valid is true

    agent_valid = true;
}

function agentGoBack() {
    var x = document.getElementById("Agent-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[1].style.display = "inline";
    document.getElementsByClassName("go-back")[1].style.display = "none";
    agent_valid = false;
}

/****************** Map parameters form *******************/

var map_params_valid = false;

function mapParamsAccept() {
    var x = document.getElementById("Map-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }
    document.getElementsByClassName("accept")[2].style.display = "none";
    document.getElementsByClassName("go-back")[2].style.display = "inline";

    //TODO: Check if form is all right then flag map_params_valid is true

    map_params_valid = true;
}

function mapParamsGoBack() {
    var x = document.getElementById("Map-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[2].style.display = "inline";
    document.getElementsByClassName("go-back")[2].style.display = "none";
    map_params_valid = false;
}

/****************** Time parameters form *******************/

var time_params_valid = false;

function timeParamsAccept() {
    var x = document.getElementById("Time-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }
    document.getElementsByClassName("accept")[3].style.display = "none";
    document.getElementsByClassName("go-back")[3].style.display = "inline";

    //TODO: Check if form is all right then flag time_params_valid is true

    time_params_valid = true;
}

function timeParamsGoBack() {
    var x = document.getElementById("Time-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[3].style.display = "inline";
    document.getElementsByClassName("go-back")[3].style.display = "none";
    time_params_valid = false;
}

/****************** AJAX - asynchronous exchange of data with a web server behind the scenes *******************/

/****************** Submitting forms *******************/
//TODO
function submitForms() {
    if (cities_valid == true && agent_valid == true && map_params_valid == true && time_params_valid == true) {

        var xhttp;
        xhttp = new XMLHttpRequest();

        //TODO: Using url parameter to deciade which action should be executed on server-side
        xhttp.open("POST", url, true);
        if (url == "action.php") { //or switch?
            var headerName = "Content-type";
            var headerValue = "application/x-www-form-urlencoded";
            var sendString = "fname=Henry&lname=Ford";
            xhttp.setRequestHeader(headerName, headerValue);
            xhttp.send(sendString);
        }

        //Some notes only for me ;)  :
        //var x = document.getElementById("myForm").elements.namedItem("fname").value;
        //document.getElementById("demo").innerHTML = "Forms submited";
    } else {
        alert("Nie zatwierdzono wszystkich formularzy!");
        return false;
    }
}


//Request with Response communication and service
/*
//Example of use in HTML
//<button type="button" onclick="loadDoc('ajax_info.txt', myFunction)">Change Content</button>
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