cmake_minimum_required(VERSION 3.16)

project(MauiManGroup)

set(MAUIMAN_VERSION 1.0.2)

add_subdirectory(lib) #adds mauiman libs

if(UNIX AND NOT APPLE AND NOT ANDROID)
add_subdirectory(server) #adds and installs the dbus service and is a server
endif()
