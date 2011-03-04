set (tp_qt4_logger_INCLUDE_PATHS
     /usr/include/telepathy-logger-0.1
     /usr/include/telepathy-1.0
     /usr/include/libxml2
     /usr/include/dbus-1.0
     /usr/include/glib-2.0
     /usr/lib64/glib-2.0/include
     /usr/lib64/dbus-1.0/include
     /usr/include/QtGStreamer
     /usr/include/QtGui
)

add_definitions(-DQT_NO_KEYWORDS)

include_directories (${PROJECT_SOURCE_DIR} ${tp_qt4_logger_INCLUDE_PATHS})

# Make sure the compiler can find include files from our Hello library.
include_directories (${PROJECT_SOURCE_DIR})

# Make sure the linker can find the Hello library once it is built.
link_directories (${HELLO_BINARY_DIR}/QTpLogger)

# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable (QTpLogger-test dummy-main.cpp)

# Link the executable to the Hello library.
target_link_libraries (QTpLogger-test telepathy-qt4-logger)
