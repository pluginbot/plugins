#-------------------------------------------------
# Configuration file used to set the paths and other varibles
#------------------------------------------------

# The BASEDIR is the root of the project
BASEDIR = $$PWD

# The LIBDIR is where the libraries are located
LIBDIR = $$quote($${BASEDIR}/Libs)

# The INTERFACEDIR is where the interfaces are located
INTERFACEDIR = $$quote($${BASEDIR}/Interfaces)

# The DISTDIR is where the finished application will be placed
DISTDIR = $$quote($${BASEDIR}/../PluginbotSDK)

# The DISTPLUGINDIR is where the finished plugins will be placed
DISTPLUGINDIR = $$quote($${DISTDIR}/Plugins)
