include (${QT_USE_FILE})

include_directories (${PROJECT_SOURCE_DIR})

include_directories (${PROJECT_SOURCE_DIR} /usr/include/QtGStreamer)

link_directories (${PROJECT_BINARY_DIR}/QTpLogger)

add_executable (QTpLogger-test dummy-main.cpp)

target_link_libraries (QTpLogger-test telepathy-logger-qt4 ${QT_LIBRARIES})
