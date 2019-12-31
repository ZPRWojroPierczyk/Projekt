import platform, os

Import('env', 'DEBUG', 'BOOST_LIB_PATH')

DEBUG = int(DEBUG)

#----------------------------------------------------------
#---------------------- Environment -----------------------
#----------------------------------------------------------

if(platform.system() == "Linux"):
    env.Append( CPPPATH = [ Dir('#include/server') ] )
    env.Append( LIBPATH = [] )
    env.Append( LIBS = ['boost_thread', 'boost_program_options'] )
    # Custom compiller flags
    env.Append( CPPFLAGS = ' -pthread' )
    # Custom linker flags
    # -- -Wl,-R defines default search path to the 
    #    shared libraries for the result app
    #    (thanks to using it, user has not to define
    #    LD_LIBRARY_PATH while running app)
    #
    #    WARNING : For some reason g++ needs a single
    #              '-Wl,...' per a shared library to
    #              make a fine linkage
    env.Append( LINKFLAGS = ' -pthread -Wl,-R $BOOST_LIB_PATH -Wl,-R $BOOST_LIB_PATH')

    # Debug-dependant configuration
    if DEBUG == 0:
        pass
    elif DEBUG == 1:
        pass

elif(platform.system() == "Windows"):
    env.Append( CPPPATH = [ Dir('#include\\server') ] )
    env.Append( LIBPATH = [] )
    
    # Custom compiller flags
    env.Append( CPPFLAGS = '' )
    # Custom linker flags
    env.Append( LINKFLAGS = '')

    # Debug-dependant configuration
    if DEBUG == 0:
        pass
    elif DEBUG == 1:
        pass

#----------------------------------------------------------
#-------------------- Build and Install -------------------
#----------------------------------------------------------

app = env.Program( source = [Glob('*.cc'), Glob('*/*.cc')], target = 'app' )

if DEBUG == 0:
    env.Install('#bin/release/app', app)
    env.Alias('install', '#bin/release/app')
elif DEBUG == 1:
    env.Install('#bin/debug/app', app)
    env.Alias('install', '#bin/debug/app')