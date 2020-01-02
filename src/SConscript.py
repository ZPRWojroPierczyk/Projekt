import platform, os

Import('env', 'DEBUG')
Import('LINUX_BOOST_LIB_PATH')
DEBUG = int(DEBUG)
env_app = env.Clone()

#----------------------------------------------------------
#---------------------- env_appironment -----------------------
#----------------------------------------------------------

if(platform.system() == "Linux"):
    env_app.Append( CPPPATH = [ ] )
    env_app.Append( LIBPATH = [ ] )
    env_app.Append( LIBS = ['boost_thread', 'boost_program_options'] )
    # Custom compiller flags
    env_app.Append( CPPFLAGS = ' -pthread' )
    # Custom linker flags
    # -- -Wl,-rpath defines default search path to the 
    #    shared libraries for the result app
    #    (thanks to using it, user has not to define
    #    LD_LIBRARY_PATH while running app)
    env_app.Append( LINKFLAGS = ' -pthread -Wl,-rpath=' + LINUX_BOOST_LIB_PATH)

    # Debug-dependant configuration
    if DEBUG == 0:
        pass
    elif DEBUG == 1:
        pass

elif(platform.system() == "Windows"):
    env_app.Append( CPPPATH = [ ] )
    env_app.Append( LIBPATH = [ ] )
    
    # Custom compiller flags
    env_app.Append( CPPFLAGS = '' )
    # Custom linker flags
    env_app.Append( LINKFLAGS = '')

    # Debug-dependant configuration
    if DEBUG == 0:
        pass
    elif DEBUG == 1:
        pass

#----------------------------------------------------------
#-------------------- Build and Install -------------------
#----------------------------------------------------------

obj_list = env_app.StaticObject(source = [Glob('*/*.cc'), Glob('*/*/*.cc'), Glob('*/*/*/*.cc')])
app = env_app.Program( source = ['main.cc', obj_list], target = 'app' )

if DEBUG == 0:
    env_app.Install('#bin/release/app', app)
    env_app.Alias('install', '#bin/release/app')
elif DEBUG == 1:
    env_app.Install('#bin/debug/app', app)
    env_app.Alias('install', '#bin/debug/app')

Return('obj_list')