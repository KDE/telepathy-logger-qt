# Try to find the Qt binding of the Telepathy library
# TELEPATHY_QT_YELL_FOUND - system has Telepathy-Qt
# TELEPATHY_QT_YELL_INCLUDE_DIR - the Telepathy-Qt include directory
# TELEPATHY_QT_YELL_LIBRARIES - Link these to use Telepathy-Qt

# Copyright (c) 2011 Collabora Ltd <http://www.collabora.co.uk>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set (TELEPATHY_QT4_YELL_FIND_REQUIRED ${TelepathyQt4Yell_FIND_REQUIRED})
if (TELEPATHY_QT4_YELL_INCLUDE_DIR AND TELEPATHY_QT4_YELL_LIBRARIES)
  # Already in cache, be silent
  set (TELEPATHY_QT4_YELL_FIND_QUIETLY TRUE)
endif (TELEPATHY_QT4_YELL_INCLUDE_DIR AND TELEPATHY_QT4_YELL_LIBRARIES)

find_path (TELEPATHY_QT4_YELL_INCLUDE_DIR
          NAMES TelepathyQt4Yell/Types TelepathyQt4Yell/Models/ConversationModel
          PATHS ${CMAKE_INSTALL_PREFIX}/include/telepathy-1.0
)
find_library (TELEPATHY_QT4_YELL_LIBRARIES
             NAMES telepathy-qt4-yell
             PATHS ${CMAKE_INSTALL_PREFIX}/lib
)

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (TELEPATHY_QT4_YELL DEFAULT_MSG
                                   TELEPATHY_QT4_YELL_LIBRARIES TELEPATHY_QT4_YELL_INCLUDE_DIR)
