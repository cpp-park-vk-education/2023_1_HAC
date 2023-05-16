FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(include ../include ${GTEST_INCLUDE_DIRS})
#FILE(GLOB GUI_LIB ../../project/src/*)

ADD_EXECUTABLE(
        tests
        src/main.cpp
        src/gui_authorization_tests.cpp
        src/gui_registration_tests.cpp
        src/gui_user_settings_tests.cpp
        src/gui_mainwindow_tests.cpp

        ../src/authorizationwindow_network.cpp
        ../src/usecase_authorization_window.cpp

        ../src/registrationwindow_network.cpp
        ../src/usecase_registrationwindow.cpp

        ../src/mainwindow_network.cpp
        ../src/usecase_mainwindow.cpp

        ../src/usersettingswindow_network.cpp
        ../src/usecase_usersettingswindow.cpp
)
TARGET_LINK_LIBRARIES(tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}  -lstdc++fs)

SET(memcheck_command)

IF (WITH_MEMCHECK)
    MESSAGE("Memcheck enabled")
    SET(memcheck_command ${CMAKE_SOURCE_DIR}/memtest.sh)
ELSE()
    MESSAGE("Memcheck disabled")
ENDIF()

SET(TEST_COMMAND ${memcheck_command} ${CMAKE_BINARY_DIR}/project/tests/tests)

ADD_TEST(
        NAME QtTest
        COMMAND ${TEST_COMMAND}
)
