# Authors
Krzysztof Pierczyk, Wojciech Rokicki

# Version
0.0.1

# Usage (OUT-OF-DATE)
Currently implemented python-HTTPServer can be run without
installing any non-standard libraries. Server runs at the
0.0.0.0:PORT socket (where PORT can be set in the 
/config/python_server.conf file) and can be reached via
web browser.

# Server requests

## Creator

GET requests - requesting page to load

POST requests - sending data to server (without response):

```
//Cities form data
var headerName = "Content-type";
var headerValue = "creator/cities";
var sendString = json_form_data;
```

```
//Transports form data
var headerName = "Content-type";
var headerValue = "creator/transports";
var sendString = json_form_data;
```
```
//Map parameters form data
var headerName = "Content-type";
var headerValue = "creator/map-params";
var sendString = json_form_data;
```

```
//Time parameters form data
var headerName = "Content-type";
var headerValue = "creator/time-params";
var sendString = json_form_data;
```

```
//Simulation transition, all forms acceptance confirmation
var headerName = "Content-type";
var headerValue = "creator/success";
```

## Simulation

GET requests - requesting page to load

POST requests:

```
//Requesting time parameters to create slider
var headerName = "Content-type";
var headerValue = "simulation/start";
```

```
//Requesting snapshot of simulation
var headerName = "Content-type";
var headerValue = "simulate/snapshot";
var sendString = json_snapshot_request;
```

```
//Requesting updated current snapshot and resimulation
var headerName = "Content-type";
var headerValue = "simulation/update";
var sendString = json_update_request;
```