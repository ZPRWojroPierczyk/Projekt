#ifndef RESOURCES_DISPATCHER_H
#define RESOURCES_DISPATCHER_H

/*
* ResourcesDispatcher class is designed to manage requests
* from HTTPServer (@see HTTPServer) concerning resource that
* should be sent to the client (html, css, js, graphics,
* audio, etc.)
*
* IMPORTANT:
*
* Resources management does not take into account logical aspects
* of the application. It provides only buffer between HTTPServer
* and resources bank that picks appropriate items depending on the
* application's state.
*/

class ResourcesDispatcher{
    
};

#endif
