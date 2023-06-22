# Copyright 2018-2020 Camilo Higuita <milo.h@aol.com>
# Copyright 2018-2020 Nitrux Latinoamericana S.C.
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

project(MauiMan VERSION ${MAUIMAN_VERSION})

##CMAKE PARTS
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/MauiMan${MAUI_MAJOR_VERSION}")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MauiManConfig.cmake.in.${QT_MAJOR_VERSION}"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiMan${MAUI_MAJOR_VERSION}Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX MauiMan
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/mauiman_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/MauiMan${MAUI_MAJOR_VERSION}ConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MauiMan${MAUI_MAJOR_VERSION}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiMan${MAUI_MAJOR_VERSION}ConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT MauiMan${MAUI_MAJOR_VERSION}Targets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE MauiMan${MAUI_MAJOR_VERSION}Targets.cmake
    NAMESPACE MauiMan${MAUI_MAJOR_VERSION}::)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Core Gui)

if(UNIX AND NOT APPLE AND NOT ANDROID)
    find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS DBus)

    if (QT_MAJOR_VERSION STREQUAL "5")
        find_package(Qt5SystemInfo 5.4.0 REQUIRED)
        set_package_properties(Qt5SystemInfo PROPERTIES
            TYPE REQUIRED
            DESCRIPTION "Qt Systems."
            URL "https://github.com/qt/qtsystems"
            PURPOSE "Qt5SystemInfo is needed to detect the system form factor based on the input methods."
        )
endif()
endif()

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
