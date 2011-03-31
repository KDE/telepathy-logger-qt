# Some useful commands
execute_process(COMMAND ${SH} tools/git-which-branch.sh misc | tr -d '\n' | tr -C "[:alnum:]" _
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_BRANCH_CURRENT)

if (GIT_BRANCH_CURRENT)
    string(LENGTH ${GIT_BRANCH_CURRENT} HAVE_GIT_BRANCH)

    if (HAVE_GIT_BRANCH)
        string(REPLACE "\n" "" GIT_BRANCH_CURRENT ${GIT_BRANCH_CURRENT})

        set(UPLOAD_BRANCH_TO people.freedesktop.org:public_html/${PACKAGE_NAME})

        add_custom_target(upload-branch-docs rsync -rtzvPp --chmod=a+rX doc/html/ ${UPLOAD_BRANCH_TO}-${GIT_BRANCH_CURRENT}
                                            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_dependencies(upload-branch-docs doxygen-doc)
    endif (HAVE_GIT_BRANCH)
endif (GIT_BRANCH_CURRENT)

add_custom_target(maintainer-upload-release-docs
        rsync -rtOvzPp --chmod=Dg+s,ug+rwX,o=rX doc/html/ telepathy.freedesktop.org:/srv/telepathy.freedesktop.org/www/doc/${PACKAGE_NAME}/

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_dependencies(maintainer-upload-release-docs doxygen-doc _maintainer-upload-release-check)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/maintainer-upload-release-check.sh "
#!/bin/sh
case ${PACKAGE_VERSION} in
        (*.*.*.*)
                echo \"${PACKAGE_VERSION} is not a release\" >&2;
                exit 2;
                ;;
esac
test -f ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
if ! test -f ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz.asc; then
    gpg --detach-sign -a ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz;
fi;
gpg --verify ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz.asc
")

add_custom_target(_maintainer-upload-release-check ${SH} ${CMAKE_CURRENT_BINARY_DIR}/maintainer-upload-release-check.sh)

add_custom_target(maintainer-upload-release
        rsync -vzP ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
telepathy.freedesktop.org:/srv/telepathy.freedesktop.org/www/releases/${PACKAGE_NAME}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
        COMMAND
        rsync -vzP ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz.asc
telepathy.freedesktop.org:/srv/telepathy.freedesktop.org/www/releases/${PACKAGE_NAME}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz.asc
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_dependencies(maintainer-upload-release _maintainer-upload-release-check maintainer-upload-release-docs)


set(toolchain_files
    c-constants-gen.py
    check-misc.sh
    check-whitespace.sh
    git-which-branch.sh
    glib-ginterface-gen.py
    glib-gtypes-generator.py
    glib-interfaces-gen.py
    glib-signals-marshal-gen.py
    libtpcodegen.py
    libglibcodegen.py
    libqt4codegen.py
    qt4-client-gen.py
    qt4-constants-gen.py
    qt4-types-gen.py
    manager-file.py
    with-session-bus.sh
    xincludator.py
)

string(REPLACE "." " " sh_toolchain_files ${toolchain_files})

set(TELEPATHY_SPEC_SRCDIR ${CMAKE_SOURCE_DIR}/../telepathy-spec)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/maintainer-update-from-telepathy-spec.sh "
#!/bin/sh
set -e
cd ${CMAKE_SOURCE_DIR}
for x in ${sh_toolchain_files}; do
    if test -f ${TELEPATHY_SPEC_SRCDIR}/tools/$$x; then
        cp ${TELEPATHY_SPEC_SRCDIR}/tools/$$x $$x;
    fi;
done
")
add_custom_target(maintainer-update-from-telepathy-spec ${SH} ${CMAKE_CURRENT_BINARY_DIR}/maintainer-update-from-telepathy-spec.sh
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

set(TELEPATHY_GLIB_SRCDIR ${CMAKE_SOURCE_DIR}/../telepathy-glib)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/maintainer-update-from-telepathy-glib.sh "
#!/bin/sh
set -e
cd ${CMAKE_SOURCE_DIR}
for x in ${sh_toolchain_files}; do
    if test -f ${TELEPATHY_GLIB_SRCDIR}/tools/$$x; then
        cp ${TELEPATHY_GLIB_SRCDIR}/tools/$$x $$x;
    fi;
done
")
add_custom_target(maintainer-update-from-telepathy-glib ${SH} ${CMAKE_CURRENT_BINARY_DIR}/maintainer-update-from-telepathy-glib.sh
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

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
