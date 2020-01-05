//TODO: 
//Number input validation - decimal, integer
//JSON data generation and submiting
//Think time-window and time-step parameters over

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

/****************** Cities form validation *******************/

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
            if(x.elements[i+1].value < 0 || x.elements[i+2].value < 0) {
                alert(element.getAttribute("data-name")
                 + " niepoprawnie uzupełniony. Liczba pojazdów i kierowców musi być nieujemna.\
                  Proszę wprowadzić poprawne dane ponownie.");
                return;
            }
            checked_count++;
        }
    }

    //At least 2 cities must be checked
    if(checked_count >= 2){
        for (i = 0; i < x.length; i++) {
            x.elements[i].disabled = true;
        }
        document.getElementsByClassName("accept")[0].style.display = "none";
        document.getElementsByClassName("go-back")[0].style.display = "inline";
        cities_valid = true;
    } else {
        alert("Proszę wybrać conajmniej 2 miasta.");
    }

    //Debug purposes
    //document.getElementById("demo").innerHTML = "Ilość zaznaczonych miast: "
    // + checked_count + ". Czy formularz uzupełniony poprawnie: " + cities_valid;
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
    
    //Debug purposes
    //document.getElementById("demo").innerHTML = "Czy formularz uzupełniony poprawnie: " + cities_valid;

}

/****************** Transport tab entry condition and form generator *******************/

var transport_elements_count = 0;

function transportEntry(evt, tab) {
    if(cities_valid == true) {
        if(transport_elements_count == 0){
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
    if(transport_elements_count == 0){
        document.getElementsByClassName("accept")[1].style.display = "inline";
    }

    var html_content = "<tr><td>Z miasta: ";
    var html_select = "<select>";
    var x = document.getElementById("Cities-Form");
    
    for (i = 0; i < x.length; i++) {
        var element = x.elements[i];
        if (element.type == "checkbox" && element.checked == true) {
            html_select += "<option value=\"" + element.value + "\">" + element.getAttribute("data-name") + "</option>";
        }
    }

    html_select += "</select>";

    html_content += html_select + "</td><td>Do miasta: " + html_select + "</td><td>"
    + "<input type=\"number\" value=\"0\" step=\"1\" min=\"0\">  kg</td>"
    + "<td><button class=\"remove-transport-element-button\" onclick=\"return transportRemoveElement(event);\">\
    Usuń</button></td></tr>";

    document.getElementById("Transport-Table").innerHTML += html_content;
    transport_elements_count++;
}

function transportRemoveElement(evt) {
    evt.currentTarget.parentElement.parentElement.remove();
    transport_elements_count--;
    if(transport_elements_count == 0){
        document.getElementsByClassName("accept")[1].style.display = "none";
    }
    return false;
}


/****************** Transport form validation *******************/

var transport_valid = false;

function transportAccept() {
    var select_distinction = 0;
    var x = document.getElementById("Transport-Form");
    var i;

    for (i = 0; i < x.length; i++) {
        if(x.elements[i].type == "number" && x.elements[i].value < 0){
            alert("Ilość towaru musi być dodatnia.");
            return;
        }
        if(x.elements[i].tagName == "SELECT"){
            if(select_distinction == 0){
                if(x.elements[i].value == x.elements[i+1].value){
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
        if(x.elements[i].tagName == "BUTTON"){
            x.elements[i].style.display = "none";
        }
        x.elements[i].disabled = true;
    }
    
    document.getElementById("Generate-Transport-Button").style.display = "none";
    document.getElementsByClassName("accept")[1].style.display = "none";
    document.getElementsByClassName("go-back")[1].style.display = "inline";

    transport_valid = true;
}

function transportGoBack() {
    var x = document.getElementById("Transport-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        if(x.elements[i].tagName == "BUTTON"){
            x.elements[i].style.display = "inline";
        }
        x.elements[i].disabled = false;
    }
    document.getElementById("Generate-Transport-Button").style.display = "inline";
    document.getElementsByClassName("accept")[1].style.display = "inline";
    document.getElementsByClassName("go-back")[1].style.display = "none";
    transport_valid = false;
}

/****************** Agent form validation *******************/

var agent_valid = false;

function agentAccept() {
    var x = document.getElementById("Agent-Form");
    var i;
    if(x.elements[0].value < 0 || x.elements[0].value > 100){
        alert("Prawdopodobieństwo nie może być ujemne, ani nie może przekraczać 100%.");
        return;
    }
    for (i = 1; i < x.length; i++) {
        if(x.elements[i].value <= 0){
            alert("Wszystkie parametry, oprócz prawdopodobieństwa wypadku, muszą być dodatnie.");
            return;
        }
    }
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }
    document.getElementsByClassName("accept")[2].style.display = "none";
    document.getElementsByClassName("go-back")[2].style.display = "inline";

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

/****************** Map parameters form validation *******************/

var map_params_valid = false;

function mapParamsAccept() {
    var x = document.getElementById("Map-Params-Form");
    var i;

    if(x.elements[0].value < 0 || x.elements[0].value > 100
        || x.elements[1].value < 0 || x.elements[1].value > 100){
        alert("Prawdopodobieństwo nie może być ujemne, ani nie może przekraczać 100%.");
        return;
    }

    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }

    document.getElementsByClassName("accept")[3].style.display = "none";
    document.getElementsByClassName("go-back")[3].style.display = "inline";

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

/****************** Time parameters form validation *******************/

var time_params_valid = false;

function timeParamsAccept() {
    var x = document.getElementById("Time-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = true;
    }

    if(x.elements[0].value <= 0 || x.elements[1].value <= 0){
        alert("Parametry muszą być dodatnie.");
        return;
    }

    document.getElementsByClassName("accept")[4].style.display = "none";
    document.getElementsByClassName("go-back")[4].style.display = "inline";

    time_params_valid = true;
}

function timeParamsGoBack() {
    var x = document.getElementById("Time-Params-Form");
    var i;
    for (i = 0; i < x.length; i++) {
        x.elements[i].disabled = false;
    }
    document.getElementsByClassName("accept")[4].style.display = "inline";
    document.getElementsByClassName("go-back")[4].style.display = "none";
    time_params_valid = false;
}

/************** AJAX - asynchronous exchange of data with a web server behind the scenes ***************/

/****************** Submitting forms *******************/
//TODO
function submitForms() {
    if (cities_valid == true && transport_valid == true && agent_valid == true 
        && map_params_valid == true && time_params_valid == true) {

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