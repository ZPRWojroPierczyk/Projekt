import platform
import os

#----------------------------------------------------------
#---------------------- Preparation -----------------------
#----------------------------------------------------------

LINUX_BOOST_INCLUDE_PATH = '/usr/local/include/boost'
LINUX_BOOST_LIB_PATH = '/usr/local/lib'
#LINUX_BOOST_INCLUDE_PATH = 'include/boost'
#LINUX_BOOST_LIB_PATH = 'lib'

LINUX_SQLITE3_INCLUDE_PATH = '/usr/inlude'
LINUX_SQLITE3_LIB_PATH = '/usr/lib/x86_64-linux-gnu'

LINUX_SSL_INCLUDE_PATH = '/usr/inlude'
LINUX_SSL_LIB_PATH = '/usr/lib/x86_64-linux-gnu'

LINUX_CRYPTO_INCLUDE_PATH = '/usr/inlude'
LINUX_CRYPTO_LIB_PATH = '/usr/lib/x86_64-linux-gnu'

#----------------------------------------------------------

LINUX_CPPPATH = [ \
    Dir(LINUX_BOOST_INCLUDE_PATH), \
	Dir(LINUX_SQLITE3_INCLUDE_PATH), \
	Dir(LINUX_SSL_INCLUDE_PATH), \
	Dir(LINUX_CRYPTO_INCLUDE_PATH) \
]
LINUX_LIBPATH = [ \
	Dir(LINUX_BOOST_LIB_PATH), \
	Dir(LINUX_SQLITE3_LIB_PATH), \
	Dir(LINUX_SSL_LIB_PATH), \
	Dir(LINUX_CRYPTO_LIB_PATH) \
]

#----------------------------------------------------------
#------------- Get include directories list ---------------
#----------------------------------------------------------

# Get path to src folder
includePath = str(Dir('include').srcnode().abspath)
# List all subdirectories in include/
for root, dirnames, filenames in os.walk(includePath):
	LINUX_CPPPATH.append(Dir(root))

#----------------------------------------------------------
#---------------------- Preparation -----------------------
#----------------------------------------------------------

# Check if debug build (command-line argument 'debug=1')
# Default value for debug: 0
debug = int(ARGUMENTS.get('debug', 0))

# Export debug to submodules
Export({'DEBUG': debug})

# Empty environment
env = Environment()

#----------------------------------------------------------
#---------- Src/test folder depths calculation ------------
#----------------------------------------------------------

# Depths are used to establish number of Glob() objects
# passed to the builders, where every Glob represents
# */*/.../*.cc pattern for an appropriate depth in src/test
# folder's structure

def getDepth(path, depth=0):
    if not os.path.isdir(path): return depth
    maxdepth = depth
    for entry in os.listdir(path):
        fullpath = os.path.join(path, entry)
        maxdepth = max(maxdepth, getDepth(fullpath, depth + 1))
    return maxdepth

srcDepth = getDepth('src')
testDepth = getDepth('test')

#----------------------------------------------------------
#--------------- Linux Common Environment -----------------
#----------------------------------------------------------

if(platform.system() == "Linux"):
	env.Append( CPPPATH = LINUX_CPPPATH )
	env.Append( LIBPATH = LINUX_LIBPATH )
	env.Append( LIBS = [] )
	# Custom compiller flags
	env.Append( CPPFLAGS = '-Wall -pedantic -std=c++17 ' )
	# Custom linker flags
	env.Append( LINKFLAGS = '-Wall ' )
	env.Append( LINKFLAGS = '-Wl,-rpath={} '.format(LINUX_BOOST_LIB_PATH))
	env.Append( LINKFLAGS = '-Wl,-rpath={} '.format(LINUX_SQLITE3_LIB_PATH))

	# Compile-time ROOT constant is used by the programm
	# to be aware about it's structure's localization
	env.Append( CPPFLAGS = '-D ROOT=\\\"{}\\\" '.format(Dir('.').abspath) )
	env.Append( CPPFLAGS = '-D API_KEY=\\\"AIzaSyCjAs8A6v5EcrU6ccJ-W0WPe3mbdNS9Uik\\\" ' )

	# Debug-dependant configuration
	if debug == 1:
		env.Append( CPPFLAGS = '-g ')
		env.Append( CPPFLAGS = '-D DEBUG ')

	# Export required variables to submodules
	Export('env')

#----------------------------------------------------------
#-------------- Windows Common Environment ----------------
#----------------------------------------------------------

elif(platform.system() == "Windows"):
	env.Append( CPPPATH = [ Dir( ) ] )
	env.Append( LIBPATH = [ Dir( ) ] )
	
	# Custom compiller flags
	env.Append( CPPFLAGS = ' /EHsc /MD /D "WIN32" /D "_CONSOLE" /W4' )
	# Custom linker flags
	env.Append( LINKFLAGS = [ ] )

	# Debug-dependant configuration
	if debug == 1:
		pass

#----------------------------------------------------------
#------------------------- Build --------------------------
#----------------------------------------------------------

def version(debug):
	if debug == 1:
		return 'debug'
	else:
		return 'release'

# Build app
VariantDir('obj/' + version(debug) + '/app', 'src', duplicate = 0)
srcObjects = SConscript('obj/' + version(debug) + '/app/SConscript.py', exports='srcDepth')

# Build test
VariantDir('obj/' + version(debug) + '/test', 'test', duplicate = 0)
SConscript('obj/' + version(debug) + '/test/SConscript.py', exports = ['srcObjects', 'testDepth'])

#----------------------------------------------------------
#----------------------- Utilities ------------------------
#----------------------------------------------------------

# Help printed to the terminal after typing 'scons -h'
Help("""
Type: 'scons ' to build the production program,
      'scons debug=1' to build the debug version.
""")