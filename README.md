# Multiagent simulator of distribution

## Authors
Wojciech Rokicki & Krzysztof Pierczyk

## Compilation
To compile the project you have to modify paths of scons configuration file named "SConstruct.py".
Then simply type ```scons``` in the base folder, where is also located same "SConstruct.py" file.

Our project was made within the use of Boost Libraries (boost_1_72_0). If you don't have it or if you have previous version please run script ```install_boost.sh``` which is in "scripts" subfolder.

In subfolder "config", in "http_server.conf" file you can set required port and other http_server related settings.
Especially change doc_root parameter to point to "web" subfolder of the Project.

After configurations are set, you can run the application which execution file is in "bin/release/app".

You can find documentation in "doc" subfolder.
