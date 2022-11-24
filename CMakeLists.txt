cmake_minimum_required(VERSION 3.16)

project(MauiManLib)

set(MAUIMAN_VERSION 1.0.1)

add_subdirectory(mauiman) #adds mauiman libs

if(UNIX AND NOT APPLE AND NOT ANDROID)

add_subdirectory(server) #adds and installs the dbus service and is a server
endif()
