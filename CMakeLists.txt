cmake_minimum_required(VERSION 3.16)

set(MAUIMAN_VERSION 1.0.0)

add_subdirectory(mauiman) #adds mauiman libs
add_subdirectory(server) #adds and installs the dbus service and is a server
