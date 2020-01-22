# Multiagent simulator of distribution

## Authors
Wojciech Rokicki & Krzysztof Pierczyk

## Libraries installation
Our project was made within the use of Boost Libraries (boost_1_72_0). If you don't have it or if you have
previous version please run, from the main project's folder, script ```scripts/install_boost.sh```.

The same schema applies to the sqlite3 library used by the project. In both cases binaries will be installed
in the 'lib/lib_name' folder and header files in 'include/lib_name'.

## Compilation
To compile the project you first have to modify paths to dynamic libraries and libraries' header files
in the scons configuration file named "SConstruct". Then simply type ```scons``` from the base folder.

Compilation process uses '-Wl,-rpath' argument during builng so you should be able to run result 
application without setting LD_LIBRARY_PATH or moving libararies' binaries to the installation folder.

NOTE: For an unknown reason Scons have problem with finding files in 'include/' and 'lib/' folders when
given relative paths in 'SConstruct'. If this is your case, just switch paths to absolute.

## Configuration
In subfolder "config", in "http_server.conf" file you can adjust base server-related settings involving server's
IP address and ort number. Overmore you can manipulate timeouts used by the server:
    
    - client_timeout_min is a time (in minutes) that should pass
      before closing client's app instance while having no requests
      from the client.
    
    - session_timeout_s is a maximum time that server will keep TCP
      connection with a browser when no requests are sent

## Running 
After configurations are set, you can run the application which execution file is in "bin/release/app".