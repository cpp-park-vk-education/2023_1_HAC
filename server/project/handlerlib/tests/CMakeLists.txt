FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(include ${CALC_INCLUDE_DIRS})

ADD_EXECUTABLE(
        tests
        src/main.cpp
        src/handler_base_test.cpp
)
TARGET_LINK_LIBRARIES(tests ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CALC_LIBRARIES} -lstdc++fs)

SET(memcheck_command)

IF (WITH_MEMCHECK)
        MESSAGE("Memcheck enabled")
        SET(memcheck_command ${CMAKE_SOURCE_DIR}/memtest.sh)
ELSE()
        MESSAGE("Memcheck disabled")
ENDIF()

SET(TEST_COMMAND ${memcheck_command} ${CMAKE_BINARY_DIR}/project/handlerlib/tests/tests)

ADD_TEST(
        NAME HandlerTest
        COMMAND ${TEST_COMMAND}
)