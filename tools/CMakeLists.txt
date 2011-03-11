include (${QT_USE_FILE})

include_directories (${PROJECT_SOURCE_DIR})

link_directories (${PROJECT_BINARY_DIR}/TelepathyQt4Logger)

add_executable (tpl-tool tpl-tool.cpp)

target_link_libraries (tpl-tool telepathy-qt4-logger ${QT_LIBRARIES})
