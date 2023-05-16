

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)

set(PROJECT_SOURCES
        src/main.cpp
        qcustomplot.h
        qcustomplot.cpp

        src/guicontroller.cpp
        src/authorizationwindow.cpp
        src/usecase_authorizationwindow.cpp
        src/mainwindow.cpp
        src/registrationwindow.cpp
        src/usersettingswindow.cpp
        src/ionetwork.cpp
        src/usersettingswindow_network.cpp
        src/usecase_usersettingswindow.cpp
        src/authorizationwindow_network.cpp
        src/usecase_authorization_window.cpp

        src/registrationwindow_network.cpp
        src/usecase_registrationwindow.cpp

        src/mainwindow_network.cpp
        src/usecase_mainwindow.cpp

        src/guicontroller.ui
        src/authorizationwindow.ui
        src/mainwindow.ui
        src/registrationwindow.ui
        src/usersettingswindow.ui

        include/guicontroller.h
        include/guicontroller_interface.h
        include/ionetwork.h
        include/ionetwork_interface.h

        include/authorizationwindow.h
        include/authorizationwindow_interface.h
        include/usecase_authorizationwindow.h
        include/usecase_authorizationwindow_interface.h
        include/authorizationwindow_network.h
        include/authorizationwindow_network_interface.h

        include/registrationwindow.h
        include/registrationwindow_interface.h
        include/usecase_registrationwindow.h
        include/usecase_registrationwindow_interface.h
        include/registrationwindow_network.h
        include/registrationwindow_network_interface.h

        include/mainwindow.h
        include/mainwindow_interface.h
        include/usecase_mainwindow.h
        include/usecase_mainwindow_interface.h
        include/mainwindow_network.h
        include/mainwindow_network_interface.h

        include/usersettingswindow.h
        include/usersettingswindow_interface.h
        include/usecase_usersettingswindow.h
        include/usersettingswindow_network.h
        include/usersettingswindow_network_interface.h)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test_vk_project
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
else()
    if(ANDROID)
        add_library(test_vk_project SHARED
                ${PROJECT_SOURCES}
                )
    else()
        add_executable(test_vk_project
                ${PROJECT_SOURCES}
                )
    endif()
endif()

target_link_libraries(test_vk_project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::PrintSupport)

set_target_properties(test_vk_project PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(test_vk_project)
endif()

add_subdirectory(tests)
