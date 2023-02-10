# Copyright 2018-2020 Camilo Higuita <milo.h@aol.com>
# Copyright 2018-2020 Nitrux Latinoamericana S.C.
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

set(KF5_VERSION "5.70.0")
set(REQUIRED_QT_VERSION 5.15.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(MauiManServer VERSION ${MAUIMAN_VERSION})

find_package(ECM ${KF5_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(GNUInstallDirs)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(ECMSetupVersion)
include(CMakePackageConfigHelpers)
include(KDEPackageAppTemplates)

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Core DBus Gui)
find_package(Qt5SystemInfo 5.4.0 REQUIRED)

add_subdirectory(src)

configure_file(org.mauiman.Manager.service.in
    ${CMAKE_CURRENT_BINARY_DIR}/org.mauiman.Manager.service)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.mauiman.Manager.service
    DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
