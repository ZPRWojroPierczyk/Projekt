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

/****************** Cities form validation and creating JSON data *******************/

var cities_valid = false;

function citiesAccept() {
    var x = document.getElementById("Cities-Form");
    var checked_count = 0;
    var i;

    //Counting checked cities
    for (i = 0; i < x.length; i++) {
        var element = x.elements[i];
        if (element.type == "checkbox" && element.checked == true) {
            //Checking if cars and drivers numbers are correct
            if (x.elements[i + 1].value < 0 || x.elements[i + 2].value < 0
                || !isIntegral(x.elements[i + 1].value) || !isIntegral(x.elements[i + 2].value)) {
                alert(element.getAttribute("data-name")
                    + " niepoprawnie uzupełniony. Liczba pojazdów i kierowców musi być nieujemna oraz całkowita.\
                  Proszę wprowadzić poprawne dane ponownie.");
                return;
            }
            checked_count++;
        }
    }

    //TODO: Sequence of validation could be reversed for better performance ^
    //      (first check if at least 2 are selected)

    //At least 2 cities must be checked
    if (checked_count >= 2) {
        for (i = 0; i < x.length; i++) {
            x.elements[i].disabled = true;
        }
        document.getElementsByClassName("accept")[0].style.display = "none";
        document.getElementsByClassName("go-back")[0].style.display = "inline";
        var jsonCities = createCitiesJSON();
        sendJSON(jsonCities, "cities");
        cities_valid = true;
    } else {
        alert("Proszę wybrać conajmniej 2 miasta.");
    }
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

    //TODO: Cities inconsitency - could be done better, for now it's done this way.
    document.getElementById("Transport-Table").innerHTML = "";
}

function createCitiesJSON() {
    var departments = "{";
    var x = document.getElementById("Cities-Form");
    for (i = 0; i < x.length; i++) {
        var element = x.elements[i];
        if (element.type == "checkbox" && element.checked == true) {
            departments += "\"" + element.value + "\":{";
            departments += "\"carsNumber\":" + x.elements[i + 1].value + ",";
            departments += "\"driversNumber\":" + x.elements[i + 2].value + "},"

        }
    }

    departments = departments.substr(0, departments.length - 1);
    departments += "}";
    return departments;
}

/****************** Transport tab entry condition and and creating JSON data *******************/

var transport_elements_count = 0;

function transportEntry(evt, tab) {
    if (cities_valid == true) {
        if (transport_elements_count == 0) {
            document.getElementsByClassName("accept")[1].style.display = "none";
        }
        openTab(evt, tab);
        return true;
    } else {
        alert("Aby przejść do ustawień parametrów transportu należy najpierw uzupełnić miasta.");
        return false;
    }
}

function transportGenerateElement() {
    if (transport_elements_count == 0) {
        document.getElementsByClassName("accept")[1].style.display = "inline";
    }

    var html_content = "<tr><td>Z miasta: ";
    var html_select = "<select>";
    var x = document.getElementById("Cities-Form");

    for (i = 0; i < x.length; i++) {
        var element = x.elements[i];
        if (element.type == "checkbox" && element.checked == true) {
            html_select += "<option value=\"" + element.value + "\">" + element.getAttribute("data-name")
                + "</option>";
        }
    }

    html_select += "</select>";

    html_content += html_select + "</td><td>Do miasta: " + html_select + "</td><td>"
        + "<input type=\"number\" value=\"0\" step=\"100\" min=\"0\">  kg</td>"
        + "<td><button class=\"remove-transport-element-button\" onclick=\"return transportRemoveElement(event);\">\
    Usuń</button></td></tr>";

    document.getElementById("Transport-Table").innerHTML += html_content;
    transport_elements_count++;
}

function transportRemoveElement(evt) {
    evt.currentTarget.parentElement.parentElement.remove();
    transport_elements_count--;
    if (transport_elements_count == 0) {
        document.getElementsByClassName("accept")[1].style.display = "none";
    }
    return false;
}


/****************** Transport form validation and creating JSON data *******************/

var transport_valid = false;

function transportAccept() {
    var select_distinction = 0;
    var x = document.getElementById("Transport-Form");
    var i;

    for (i = 0; i < x.length; i++) {
        if (x.elements[i].type == "number" && (x.elements[i].value < 0 || !isFloat(x.elements[i].value))) {
            alert("Ilość towaru musi być dodatnia, typu całkowitego lub ułamka dziesiętnego.");
            return;
        }
        if (x.elements[i].tagName == "SELECT") {
            if (select_distinction == 0) {
                if (x.elements[i].value == x.elements[i + 1].value) {
                    alert("Niepoprawnie uzupełniony transport. Miasta muszą być różne.");
                    return;
                }
                select_distinction++;
            } else {
                select_distinction--;
            }
        }
    }

    for (i = 0; i < x.length; i++) {
        if (x.elements[i].tagName == "BUTTON") {
            x.elements[i].style.display = "none";
        }
        x.elements[i].disabled = true;
    }

    document.getElementById("Generate-Transport-Button").style.display = "none";
    document.getElementsByClassName("accept")[1].style.display = "none";
    document.getElementsByClassName("go-back")[1].style.display = "inline";
    var jsonTransports = createTransportsJSON();
    sendJSON(jsonTransports, "transports");
    transport_valid = true;
}

function transportGoBack() {
    var x = document.getElementById("Transport-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        if (x.elements[i].tagName == "BUTTON") {
            x.elements[i].style.display = "inline";
        }
        x.elements[i].disabled = false;
    }
    document.getElementById("Generate-Transport-Button").style.display = "inline";
    document.getElementsByClassName("accept")[1].style.display = "inline";
    document.getElementsByClassName("go-back")[1].style.display = "none";
    transport_valid = false;
}

function createTransportsJSON() {
    var transports = "{\"transports\":[";
    var select_distinction = 0;
    var x = document.getElementById("Transport-Form");
    for (i = 0; i < x.length; i++) {
        if (x.elements[i].tagName == "SELECT") {
            if (select_distinction == 0) {
                transports += "{\"from\":\"" + x.elements[i].value + "\",";
                transports += "\"to\":\"" + x.elements[i + 1].value + "\",";
                select_distinction++;
            } else {
                select_distinction--;
            }
        }
        if (x.elements[i].type == "number") {
            transports += "\"load\":" + x.elements[i].value + "},";
        }
    }

    transports = transports.substr(0, transports.length - 1);
    transports += "]}";
    return transports;
}

/****************** Agent form validation and creating JSON data *******************/

var agent_valid = false;

function agentAccept() {
    var x = document.getElementById("Agent-Form");
    var i;
    if (x.elements[0].value < 0 || x.elements[0].value > 100 || !isFloat(x.elements[0].value)) {
        alert("Prawdopodobieństwo nie może być ujemne, ani nie może przekraczać 100%. \
        Musi być typu całkowitego lub ułamka dziesiętnego.");
        return;
    }
    if (x.elements[1].value <= 0 || !isFloat(x.elements[1].value)){
        alert("Maksymalny czas jazdy musi być dodatni oraz typu całkowitego lub ułamka dziesiętnego.");
        return;
    }
    for (i = 2; i < x.length; i++) {
        if (x.elements[i].value <= 0 || !isIntegral(x.elements[i].value)) {
            alert("Wszystkie parametry, oprócz prawdopodobieństwa wypadku oraz czasu jazdy bez przerwy,\
             muszą być całkowite. Muszą być też dodatnie.");
            return;
        }
    }
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }
    document.getElementsByClassName("accept")[2].style.display = "none";
    document.getElementsByClassName("go-back")[2].style.display = "inline";
    var jsonAgent = createAgentJSON();
    sendJSON(jsonAgent, "agent");
    agent_valid = true;
}

function agentGoBack() {
    var x = document.getElementById("Agent-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[2].style.display = "inline";
    document.getElementsByClassName("go-back")[2].style.display = "none";
    agent_valid = false;
}

function createAgentJSON() {
    var agent = "{\"agent\":{"
    var x = document.getElementById("Agent-Form");

    agent += "\"maxfunctionProbability\":" + x.elements[0].value + ",";
    agent += "\"maxDrivingTime\":" + x.elements[1].value + ",";
    agent += "\"breakTime\":" + x.elements[2].value + ",";
    agent += "\"maxLoad\":" + x.elements[3].value + ",";
    agent += "\"maxSpeed\":" + x.elements[4].value + "}}";
    return agent;
}

/****************** Map parameters form validation and creating JSON data *******************/

var map_params_valid = false;

function mapParamsAccept() {
    var x = document.getElementById("Map-Params-Form");
    var i;

    if (x.elements[0].value < 0 || x.elements[0].value > 100
        || x.elements[1].value < 0 || x.elements[1].value > 100
        || !isFloat(x.elements[0].value) || !isFloat(x.elements[1].value)) {
        alert("Prawdopodobieństwo nie może być ujemne, ani nie może przekraczać 100%. \
        Musi być typu całkowitego lub ułamka dziesiętnego.");
        return;
    }

    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }

    document.getElementsByClassName("accept")[3].style.display = "none";
    document.getElementsByClassName("go-back")[3].style.display = "inline";
    var jsonMapParams = createMapParamsJSON();
    sendJSON(jsonMapParams, "map-params");
    map_params_valid = true;
}

function mapParamsGoBack() {
    var x = document.getElementById("Map-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[3].style.display = "inline";
    document.getElementsByClassName("go-back")[3].style.display = "none";
    map_params_valid = false;
}

function createMapParamsJSON() {
    var map_params = "{\"mapParams\":{"
    var x = document.getElementById("Map-Params-Form");

    map_params += "\"congestionProbability\":" + x.elements[0].value + ",";
    map_params += "\"accidentProbability\":" + x.elements[1].value + "}}";
    return map_params;
}

/************** AJAX - asynchronous exchange of data with a web server behind the scenes ***************/
/******************                          Submitting forms                        *******************/

function submitForms() {
    if (cities_valid == true && transport_valid == true && agent_valid == true
        && map_params_valid == true) {
       
       var xhttp;
       xhttp = new XMLHttpRequest();
       var url = "";
       xhttp.open("POST", url, true);
       var headerName = "Content-type";
       var headerValue = "creator-success";
       xhttp.setRequestHeader(headerName, headerValue);
       xhttp.send();

        return true;
    } else {
        alert("Nie zatwierdzono wszystkich formularzy!");
        return false;
    }
}

/************** Sending JSON string in AJAX communication from particular forms ***************/

function sendJSON(jsonData, suffix) {
    var xhttp;
    xhttp = new XMLHttpRequest();
    var url = "";
    xhttp.open("POST", url, true);
    var headerName = "Content-type";
    var headerValue = "creator/" + suffix;
    var sendString = jsonData;
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send(sendString);
}

/************** Checking if a number is an integral ***************/

function isIntegral(number) {
    var pattern = /[.,]/g;
    var snumber = number.toString();

    if (snumber.search(pattern) == -1 && snumber != "") {
        return true;
    } else {
        return false;
    }
}

/************** Checking if a number is a float ***************/

function isFloat(number) {
    var snumber = number.toString();
    var dot_pattern = /[.]/g;
    var comma_pattern = /[,]/g;
    var dot_match = snumber.match(dot_pattern);
    var comma_match = snumber.match(comma_pattern);

    if (snumber == "") {
        return false;
    }

    if (dot_match == null && comma_match == null) {
        return true;
    }

    if (dot_match == null) {
        if (comma_match.length <= 1) {
            return true;
        } else {
            return false;
        }
    }

    if (comma_match == null) {
        if (dot_match.length <= 1) {
            return true;
        } else {
            return false;
        }
    }

    return false;
}