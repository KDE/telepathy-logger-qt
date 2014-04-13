# Copyright (C) 2001-2009 Kitware, Inc.
# Copyright (C) 2011 Collabora Ltd. <http://www.collabora.co.uk/>
# Copyright (C) 2011 Nokia Corporation
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

MACRO(QT_GET_MOC_FLAGS moc_flags)
  IF(QT_VERSION_MAJOR MATCHES 4)
    QT4_GET_MOC_FLAGS(${moc_flags})
  ELSE(QT_VERSION_MAJOR MATCHES 4)
    IF(QT_VERSION_MAJOR MATCHES 5)
      QT5_GET_MOC_FLAGS(${moc_flags})
    ENDIF(QT_VERSION_MAJOR MATCHES 5)
  ENDIF(QT_VERSION_MAJOR MATCHES 4)
ENDMACRO(QT_GET_MOC_FLAGS)

MACRO(QT_CREATE_MOC_COMMAND infile outfile moc_flags moc_options)
  IF(QT_VERSION_MAJOR MATCHES 4)
    QT4_CREATE_MOC_COMMAND(${infile} ${outfile} "${moc_flags}" "${moc_options}")
  ELSE(QT_VERSION_MAJOR MATCHES 4)
    IF(QT_VERSION_MAJOR MATCHES 5)
      QT5_CREATE_MOC_COMMAND(${infile} ${outfile} "${moc_flags}" "${moc_options}")
    ENDIF(QT_VERSION_MAJOR MATCHES 5)
  ENDIF(QT_VERSION_MAJOR MATCHES 4)
ENDMACRO(QT_CREATE_MOC_COMMAND)
