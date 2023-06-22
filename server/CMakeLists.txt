# Copyright 2018-2020 Camilo Higuita <milo.h@aol.com>
# Copyright 2018-2020 Nitrux Latinoamericana S.C.
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

project(MauiManServer VERSION ${MAUIMAN_VERSION})

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Core DBus Gui)

add_subdirectory(src)

configure_file(org.mauiman.Manager.service.in.${QT_MAJOR_VERSION}
    ${CMAKE_CURRENT_BINARY_DIR}/org.mauiman.Manager${MAUI_MAJOR_VERSION}.service)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.mauiman.Manager${MAUI_MAJOR_VERSION}.service
    DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
