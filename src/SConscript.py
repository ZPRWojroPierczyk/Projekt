import platform
import os

# Import variables from parent
Import('env', 'DEBUG')

# Parse DEBUG as it is imported as string
DEBUG = int(DEBUG)

# Clone env to not make a mess for 'test' building env
envApp = env.Clone()

#----------------------------------------------------------
#--------------- Linux Source Environment -----------------
#----------------------------------------------------------

if(platform.system() == "Linux"):
    envApp.Append( CPPPATH = [ ] )
    envApp.Append( LIBPATH = [ ] )
    envApp.Append( LIBS = ['boost_thread', 'boost_program_options'] )
    # Custom compiller flags
    envApp.Append( CPPFLAGS = '' )
    # Custom linker flags
    envApp.Append( LINKFLAGS = '-pthread ' )

    # Debug-dependant configuration
    if DEBUG == 1:
        pass

#----------------------------------------------------------
#-------------- Windows Source Environment ----------------
#----------------------------------------------------------

elif(platform.system() == "Windows"):
    envApp.Append( CPPPATH = [ ] )
    envApp.Append( LIBPATH = [ ] )
    
    # Custom compiller flags
    envApp.Append( CPPFLAGS = '' )
    # Custom linker flags
    envApp.Append( LINKFLAGS = '')

    # Debug-dependant configuration
    if DEBUG == 1:
        pass

#----------------------------------------------------------
#------------- Collect list of source files ---------------
#----------------------------------------------------------

# List all *.cc sources rcursively
srcSrouces = []
for root, dirnames, filenames in os.walk(Dir('.').abspath):
    # If not folder conatining 'main.cc'
    if root != Dir('.').abspath:
        # Append Glob formula to the list
        srcSrouces.append(Glob(os.path.join(root, '*.cc')))
    
#----------------------------------------------------------
#-------------------- Build and Install -------------------
#----------------------------------------------------------

srcObjects = envApp.StaticObject(source = srcSrouces)
app = envApp.Program( source = ['main.cc', srcObjects], target = 'app' )

if DEBUG == 0:
    envApp.Install('#bin/release/app', app)
    envApp.Alias('install', '#bin/release/app')
elif DEBUG == 1:
    envApp.Install('#bin/debug/app', app)
    envApp.Alias('install', '#bin/debug/app')

Return('srcObjects')