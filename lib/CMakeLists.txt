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

project(MauiMan VERSION ${MAUIMAN_VERSION})

find_package(ECM ${KF5_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(GNUInstallDirs)
include(FeatureSummary)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(ECMGenerateQmlTypes)
include(CMakePackageConfigHelpers)
include(KDEPackageAppTemplates)
include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)

##CMAKE PARTS
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/MauiMan")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MauiManConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiManConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS  KF5_INCLUDE_INSTALL_DIR CMAKE_INSTALL_PREFIX
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MauiManConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiManConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT MauiManTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE MauiManTargets.cmake
    NAMESPACE MauiMan::)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX MauiMan
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mauiman_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/MauiManConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR}
    )

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Core Gui)
find_package(Qt5SystemInfo 5.4.0 REQUIRED)
set_package_properties(Qt5SystemInfo PROPERTIES
    TYPE REQUIRED
    DESCRIPTION "Qt Systems."
    URL "https://github.com/qt/qtsystems"
    PURPOSE "Qt5SystemInfo is needed to detect the system form factor based on the input methods."
)

if(UNIX AND NOT APPLE AND NOT ANDROID)
    find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS DBus)
endif()

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
