
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}

    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${CMAKE_CURRENT_BINARY_DIR}/modules
    )

set(mauiman_HDRS
    settingsstore.h
    modules/backgroundmanager.h
    modules/thememanager.h
    modules/screenmanager.h
    modules/formfactormanager.h
    modules/accessibilitymanager.h
    modules/inputdevicesmanager.h
    mauimanutils.h
    )

set(mauiman_SRCS
    settingsstore.cpp
    modules/backgroundmanager.cpp
    modules/thememanager.cpp
    modules/screenmanager.cpp
    modules/formfactormanager.cpp
    modules/accessibilitymanager.cpp
    modules/inputdevicesmanager.cpp
    mauimanutils.cpp
    )

add_library(${PROJECT_NAME}
    SHARED
    ${mauiman_SRCS}
    )

#add_library(MauiMan::MauiMan ALIAS MauiMan)

generate_export_header(${PROJECT_NAME} BASE_NAME MauiMan)

target_include_directories(${PROJECT_NAME}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiMan>")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

if(UNIX AND NOT APPLE AND NOT ANDROID)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt5::DBus
        Qt5SystemInfo
        )
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt5::Core    
    Qt5::Gui    
    )

install(FILES
    ${mauiman_HDRS}
    ${CMAKE_CURRENT_BINARY_DIR}/mauiman_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiMan
    COMPONENT Devel)

install(TARGETS ${PROJECT_NAME} EXPORT MauiManTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
