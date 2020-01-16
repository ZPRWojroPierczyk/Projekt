

/*function start(){
    var xhttp;
    xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var jsonResponse = this.responseText;
            jsonResponse = JSON.parse(jsonResponse);
            document.getElementById("myRange").step = jsonResponse.simulationDensity;
            //TODO: slider options generation
            ...
        }
    };

    xhttp.open("POST", "", true);
    var headerName = "Content-type";
    var headerValue = "simulation/start";
    xhttp.setRequestHeader(headerName, headerValue);
    xhttp.send();
}*/