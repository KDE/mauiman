include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

set(mauimanserver_SRCS
    main.cpp
    server.cpp
    modules/background.cpp
    modules/theme.cpp
    modules/screen.cpp
    modules/locale.cpp
    modules/formfactor.cpp
    modules/accessibility.cpp
    modules/inputdevices.cpp)

qt_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Background.xml modules/background.h Background)
qt_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Theme.xml modules/theme.h Theme)
qt_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Screen.xml modules/screen.h Screen)
qt_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.FormFactor.xml modules/formfactor.h FormFactor)
qt_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.Accessibility.xml modules/accessibility.h Accessibility)
qt_add_dbus_adaptor(DBUS_SOURCES modules/org.mauiman.InputDevices.xml modules/inputdevices.h InputDevices)

set_source_files_properties(${DBUS_SOURCES} PROPERTIES SKIP_AUTOGEN ON)

add_executable(${PROJECT_NAME}${MAUI_MAJOR_VERSION}
    ${mauimanserver_SRCS}
    ${DBUS_SOURCES})

target_include_directories(${PROJECT_NAME}${MAUI_MAJOR_VERSION}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}>")

target_include_directories(${PROJECT_NAME}${MAUI_MAJOR_VERSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

target_link_libraries(${PROJECT_NAME}${MAUI_MAJOR_VERSION}
    PRIVATE
    MauiMan${MAUI_MAJOR_VERSION}
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::DBus)

install(TARGETS ${PROJECT_NAME}${MAUI_MAJOR_VERSION} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
