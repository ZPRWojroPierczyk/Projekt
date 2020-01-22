import platform
import os

# Import variables from parent
Import('env', 'DEBUG')
Import('srcObjects')
Import('testDepth')

# Parse DEBUG as it is imported as string
DEBUG = int(DEBUG)

# Clone env to not make a mess for 'test' building env
envTest = env.Clone()

#---------------------------------------------------------
#--------------- Linux Tests Environment -----------------
#---------------------------------------------------------

if(platform.system() == "Linux"):
    envTest.Append( CPPPATH = [ ] )
    envTest.Append( LIBPATH = [ ] )
    envTest.Append( LIBS = ['boost_thread', 'boost_program_options', 'boost_unit_test_framework', 'sqlite3', 'ssl', 'crypto'] )
    # Custom compiller flags
    envTest.Append( CPPFLAGS = '' )
    # Custom linker flags
    envTest.Append( LINKFLAGS = '-pthread ' )

    # Debug-dependant configuration
    if DEBUG == 1:
        pass

#---------------------------------------------------------
#-------------- Windows Tests Environment ----------------
#---------------------------------------------------------

elif(platform.system() == "Windows"):
    envTest.Append( CPPPATH = [ ] )
    envTest.Append( LIBPATH = [ ] )
    
    # Custom compiller flags
    envTest.Append( CPPFLAGS = '' )
    # Custom linker flags
    envTest.Append( LINKFLAGS = '')

    # Debug-dependant configuration
    if DEBUG == 1:
        pass

#----------------------------------------------------------
#------------- Collect list of source files ---------------
#----------------------------------------------------------

# List all *.cc sources rcursively
testSources = []
for level in range(1, testDepth):
    testSources.append(Glob('*/' * level + '*.cc'))

#----------------------------------------------------------
#-------------------- Build and Install -------------------
#----------------------------------------------------------

test = envTest.Program( source = ['mainTest.cc', testSources, srcObjects], target = 'test' )

if DEBUG == 0:
    envTest.Install('#bin/release/test', test)
    envTest.Alias('install', '#bin/release/test')
elif DEBUG == 1:
    envTest.Install('#bin/debug/test', test)
    envTest.Alias('install', '#bin/debug/test')