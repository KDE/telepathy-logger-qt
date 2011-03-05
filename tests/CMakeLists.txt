set (tp_qt4_logger_test_INCLUDE_PATHS
      /usr/include/QtGStreamer
      /usr/include/QtGui
)

include_directories (${PROJECT_SOURCE_DIR} ${tp_qt4_logger_test_INCLUDE_PATHS})

include_directories (${PROJECT_SOURCE_DIR})

link_directories (${PROJECT_BINARY_DIR}/QTpLogger)

add_executable (QTpLogger-test dummy-main.cpp)

target_link_libraries (QTpLogger-test telepathy-qt4-logger)