file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

include (${QT_USE_FILE})

include_directories (${PROJECT_SOURCE_DIR})

link_directories (${PROJECT_BINARY_DIR}/TelepathyQt4Logger)

# Headers file moc will be run on
set(tpl_tool_MOC_SRCS tpl-tool.h)

set(tpl_tool_generated_specs_mocs tpl-tool.h)

# generate client moc files
foreach(moc_src ${tpl_tool_MOC_SRCS})
    set(generated_file _gen/${moc_src})
    string(REPLACE ".h" ".moc.hpp" generated_file ${generated_file})
    tpqt4_generate_moc_i_target_deps(${CMAKE_CURRENT_SOURCE_DIR}/${moc_src} ${CMAKE_CURRENT_BINARY_DIR}/${generated_file}
                                     ${tpl_tool_generated_specs_mocs})

    list(APPEND telepathy_qt4_logger_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${generated_file})
    string(REPLACE ".h" ".moc.hpp" moc_src ${moc_src})
    add_dependencies(telepathy-qt4-logger "moc-${moc_src}")
endforeach(moc_src ${telepathy_qt4_logger_MOC_SRCS})

add_executable (tpl-tool tpl-tool.cpp)
add_definitions (-DQT_NO_KEYWORDS)

target_link_libraries (tpl-tool telepathy-qt4-logger ${QT_LIBRARIES})
