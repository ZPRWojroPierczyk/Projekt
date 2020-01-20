# Server requests

## Creator

GET requests - requesting page to load - response with proper html

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
//Agent parameters form data
var headerName = "Content-type";
var headerValue = "creator/agent";
var sendString = json_form_data;
```

```
//Map parameters form data
var headerName = "Content-type";
var headerValue = "creator/map-params";
var sendString = json_form_data;
```

```
//Simulation transition, all forms acceptance confirmation
var headerName = "Content-type";
var headerValue = "creator-success";
```

## Simulation

GET requests - requesting page to load - response with proper html

POST request - constantly querying for snapshot  - response with snapshot:

```
//Simulation snapshot request
var headerName = "Content-type";
var headerValue = "simulation/snapshot-request";
```

POST requests - sending information about request - buttons state reflection (without response):

```
//Simulation pause
var headerName = "Content-type";
var headerValue = "simulation/pause";
```

```
//Simulation play
var headerName = "Content-type";
var headerValue = "simulation/play";
```

```
//Simulation forward
var headerName = "Content-type";
var headerValue = "simulation/forward";
```

```
//Simulation backward
var headerName = "Content-type";
var headerValue = "simulation/backward";
```

```
//Simulation fast forward
var headerName = "Content-type";
var headerValue = "simulation/fast-forward";
```

```
//Simulation fast backward
var headerName = "Content-type";
var headerValue = "simulation/fast-backward";
```

POST requests - sending information about modifications (without response):

```
//Simulation modifications
var headerName = "Content-type";
var headerValue = "simulation/modifications";
var sendString = json_data;
```