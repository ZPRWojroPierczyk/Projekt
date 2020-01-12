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

## Server requests

form_data.php - just sending form data (no response)
var headerName = "Content-type";
var headerValue = "application/x-www-form-data";
var sendString = json_form_data;

init.php - requesting initial data - simulation window and simulation step (response with proper JSON)
var headerName = "Content-type";
var headerValue = "application/x-www-init-request";

simulate.php - requesting simulation snapshot (response with proper JSON)
var headerName = "Content-type";
var headerValue = "application/x-www-snapshot-request";
var sendString = json_snapshot_request;

update.php - requesting simulation update (response with proper JSON - actual snapshot - and resimulation)
var headerName = "Content-type";
var headerValue = "application/x-www-update-request";
var sendString = json_update_request;