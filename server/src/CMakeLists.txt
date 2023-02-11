include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )


set(mauimanserver_SRCS
    main.cpp
    server.cpp
    modules/background.cpp
    modules/theme.cpp
    modules/screen.cpp
    modules/locale.cpp
    modules/formfactor.cpp
    modules/accessibility.cpp
    )

qt5_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Background.xml modules/background.h Background)
qt5_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Theme.xml modules/theme.h Theme)
qt5_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Screen.xml modules/screen.h Screen)
qt5_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.FormFactor.xml modules/formfactor.h FormFactor)
qt5_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Accessibility.xml modules/accessibility.h Accessibility)

set_source_files_properties(${DBUS_SOURCES} PROPERTIES SKIP_AUTOGEN ON)

add_executable(${PROJECT_NAME}
    ${mauimanserver_SRCS}
    ${DBUS_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiManServer>")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    MauiMan
    Qt5::Core
    Qt5::DBus
    )

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
