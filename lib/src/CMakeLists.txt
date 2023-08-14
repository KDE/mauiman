
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}/modules)

set(mauiman_HDRS
    settingsstore.h
    modules/backgroundmanager.h
    modules/thememanager.h
    modules/screenmanager.h
    modules/formfactormanager.h
    modules/accessibilitymanager.h
    modules/inputdevicesmanager.h
    mauimanutils.h)

set(mauiman_SRCS
    settingsstore.cpp
    modules/backgroundmanager.cpp
    modules/thememanager.cpp
    modules/screenmanager.cpp
    modules/formfactormanager.cpp
    modules/accessibilitymanager.cpp
    modules/inputdevicesmanager.cpp
    mauimanutils.cpp)

add_library(${PROJECT_NAME}${MAUI_MAJOR_VERSION}
    SHARED
    ${mauiman_SRCS})

generate_export_header(${PROJECT_NAME}${MAUI_MAJOR_VERSION} BASE_NAME MauiMan)

target_include_directories(${PROJECT_NAME}${MAUI_MAJOR_VERSION}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiMan${MAUI_MAJOR_VERSION}>")

target_include_directories(${PROJECT_NAME}${MAUI_MAJOR_VERSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

if(UNIX AND NOT APPLE AND NOT ANDROID)
    target_link_libraries(${PROJECT_NAME}${MAUI_MAJOR_VERSION} PRIVATE Qt${QT_MAJOR_VERSION}::DBus)

    if (QT_MAJOR_VERSION STREQUAL "5")
        target_link_libraries(${PROJECT_NAME}${MAUI_MAJOR_VERSION} PRIVATE Qt5SystemInfo)
    endif()
endif()

target_link_libraries(${PROJECT_NAME}${MAUI_MAJOR_VERSION}
    PRIVATE
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Gui)

install(FILES
    ${mauiman_HDRS}
    ${CMAKE_CURRENT_BINARY_DIR}/mauiman_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiMan${MAUI_MAJOR_VERSION}
    COMPONENT Devel)

install(TARGETS ${PROJECT_NAME}${MAUI_MAJOR_VERSION} EXPORT MauiMan${MAUI_MAJOR_VERSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
