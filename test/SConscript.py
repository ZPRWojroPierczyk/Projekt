import platform, os

Import('env', 'DEBUG')
Import('LINUX_BOOST_LIB_PATH')
Import('obj_list')
DEBUG = int(DEBUG)
env_test = env.Clone()

#----------------------------------------------------------
#---------------------- env_testironment -----------------------
#----------------------------------------------------------

if(platform.system() == "Linux"):
    env_test.Append( CPPPATH = [ ] )
    env_test.Append( LIBPATH = [ ] )
    env_test.Append( LIBS = ['boost_thread', 'boost_program_options', 'boost_unit_test_framework'] )
    # Custom compiller flags
    env_test.Append( CPPFLAGS = ' -pthread' )
    # Custom linker flags
    # -- -Wl,-rpath defines default search path to the 
    #    shared libraries for the result test
    #    (thanks to using it, user has not to define
    #    LD_LIBRARY_PATH while running test)
    env_test.Append( LINKFLAGS = ' -pthread -Wl,-rpath=' + LINUX_BOOST_LIB_PATH)

    # Debug-dependant configuration
    if DEBUG == 0:
        pass
    elif DEBUG == 1:
        pass

elif(platform.system() == "Windows"):
    env_test.Append( CPPPATH = [ ] )
    env_test.Append( LIBPATH = [ ] )
    
    # Custom compiller flags
    env_test.Append( CPPFLAGS = '' )
    # Custom linker flags
    env_test.Append( LINKFLAGS = '')

    # Debug-dependant configuration
    if DEBUG == 0:
        pass
    elif DEBUG == 1:
        pass

#----------------------------------------------------------
#-------------------- Build and Install -------------------
#----------------------------------------------------------

test = env_test.Program( source = ['test.cc', obj_list], target = 'test' )

if DEBUG == 0:
    env_test.Install('#bin/release/test', test)
    env_test.Alias('install', '#bin/release/test')
elif DEBUG == 1:
    env_test.Install('#bin/debug/test', test)
    env_test.Alias('install', '#bin/debug/test')