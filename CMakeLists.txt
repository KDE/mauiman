cmake_minimum_required(VERSION 3.16)

project(MauiManGroup)

option(BUILD_WITH_QT5 "Use Qt 5" OFF)
option(BUILD_WITH_QT6 "Use Qt 6" OFF)

if(BUILD_WITH_QT5)
    set(QT_MAJOR_VERSION 5)
elseif(BUILD_WITH_QT6)
    set(QT_MAJOR_VERSION 6)
else()
    set(QT_MAJOR_VERSION 5)
endif()

if (QT_MAJOR_VERSION STREQUAL "6")
    set(REQUIRED_QT_VERSION 6.4)
    set(REQUIRED_KF_VERSION 5.240.0)
    set(KF_MAJOR_VERSION 6)
    set(MAUI_MAJOR_VERSION 4)

    set(MAUIMAN_VERSION 4.0.0)
else()
    set(REQUIRED_QT_VERSION 5.15)
    set(REQUIRED_KF_VERSION 5.108.0)
    set(KF_MAJOR_VERSION 5)
    set(MAUI_MAJOR_VERSION 3)

    set(MAUIMAN_VERSION 3.0.1)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(ECM ${REQUIRED_KF_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(GNUInstallDirs)
include(FeatureSummary)
include(ECMSetupVersion)
include(ECMGenerateQmlTypes)
include(CMakePackageConfigHelpers)
include(KDEPackageAppTemplates)
include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)

set(QT_MAJOR_VERSION 6)
set(KF_MAJOR_VERSION 6)

#set(REQUIRED_QT_VERSION 5.15.2)
 set(REQUIRED_QT_VERSION 6.2)

set(REQUIRED_KF_VERSION 5.90.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(ECM ${REQUIRED_KF_VERSION} NO_MODULE)
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

add_subdirectory(lib) #adds mauiman libs

if(UNIX AND NOT APPLE AND NOT ANDROID)
    add_subdirectory(server) #adds and installs the dbus service and is a server
endif()
