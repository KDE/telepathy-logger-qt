# setup make dist
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
                   COMMAND git archive --format=tar --prefix=${PACKAGE_NAME}-${PACKAGE_VERSION}/ HEAD |
                           gzip > ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(create-source-working-dir
                  rm -rf ${PACKAGE_NAME}-${PACKAGE_VERSION} &&
                  gzip -df ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz &&
                  tar -xf ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar &&
                  rm ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar* &&
                  cd ${PACKAGE_NAME}-${PACKAGE_VERSION}/ &&
                  rm -rf doc && mkdir doc && cp -R ${CMAKE_BINARY_DIR}/doc/html doc/

                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                  DEPENDS ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
                  COMMENT "Generating working source dir for the dist tarball")
add_dependencies(create-source-working-dir doxygen-doc)

add_custom_target(dist-hook
                  chmod u+w ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}/ChangeLog &&
                  git log --stat > ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}/ChangeLog ||
                  git log > ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}/ChangeLog

                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                  COMMENT "Updating Changelog")
add_dependencies(dist-hook create-source-working-dir)

add_custom_target(dist tar --format=ustar -chf - ${PACKAGE_NAME}-${PACKAGE_VERSION} |
                       GZIP=--best gzip -c > ${PACKAGE_NAME}-${PACKAGE_VERSION}.tar.gz
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                  COMMENT "Generating dist tarball")
add_dependencies(dist dist-hook)

# setup make distcheck
add_custom_target(distcheck rm -rf build && mkdir build && cd build && cmake .. && make && make check
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PACKAGE_VERSION}/
                  COMMENT "Testing successful tarball build")
add_dependencies(distcheck dist)
