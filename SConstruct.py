import platform, os

# Build constants
LINUX_BOOST_INCLUDE_PATH = '/usr/local/include/boost'
LINUX_BOOST_LIB_PATH = '/usr/local/lib'

LINUX_CPPPATH = [ \
    Dir(LINUX_BOOST_INCLUDE_PATH),
    Dir('#include/'), \
    Dir('#include/server/'), \
    Dir('#include/model/'), \
    Dir('#include/model/managers/'), \
    Dir('#include/model/managers/simulation/'), \
    Dir('#include/view/'), \
    Dir('#include/controller/') \
]

LINUX_LIBPATH = [ \
	Dir(LINUX_BOOST_LIB_PATH), \
]

#----------------------------------------------------------
#---------------- Command-line arguments ------------------
#----------------------------------------------------------

# Check if debug build (command-line argument 'debug=1')
# Default value for debug: 0
debug = int(ARGUMENTS.get('debug', 0))

#----------------------------------------------------------
#------------------ Common Environment --------------------
#----------------------------------------------------------

env = Environment()

if(platform.system() == "Linux"):
	env.Append( CPPPATH = LINUX_CPPPATH )
	env.Append( LIBPATH = LINUX_LIBPATH )
	env.Append( LIBS = [] )
	# Custom compiller flags
	env.Append( CPPFLAGS = '-Wall -pedantic ' +  \
						   '-D ROOT=\\\"' + Dir('.').srcnode().abspath + '\\\"' )
	# Custom linker flags
	env.Append( LINKFLAGS = '-Wall')
	
	# Debug-dependant configuration
	if debug == 0:
		pass
	elif debug == 1:
		pass
    
elif(platform.system() == "Windows"):
	env.Append( CPPPATH = [ Dir(LINUX_BOOST_INCLUDE_PATH) ] )
	env.Append( LIBPATH = [ Dir(LINUX_BOOST_LIB_PATH) ] )
	
	# Custom compiller flags
	env.Append( CPPFLAGS = ' /EHsc /MD /D "WIN32" /D "_CONSOLE" /W4' )
	# Custom linker flags
	env.Append( LINKFLAGS = '')

	# Debug-dependant configuration
	if debug == 0:
		pass
	elif debug == 1:
		env.Append( CPPFLAGS = ' -g' )

#----------------------------------------------------------
#----------------------- Exports --------------------------
#----------------------------------------------------------

# Export build constants for the nested SCconscript
Export({'DEBUG': debug})
Export('env', 'LINUX_BOOST_LIB_PATH')

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
obj_list = SConscript('obj/' + version(debug) + '/app/SConscript.py')
# Build test
VariantDir('obj/' + version(debug) + '/test', 'test', duplicate = 0)
SConscript('obj/' + version(debug) + '/test/SConscript.py', exports = 'obj_list')

#----------------------------------------------------------
#----------------------- Utilities ------------------------
#----------------------------------------------------------

# Help printed to the terminal after typing 'scons -h'
Help("""
Type: 'scons [targets]' to build the production program,
      'scons [targets] debug=1' to build the debug version.
""")