FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)


INCLUDE_DIRECTORIES(include ${HANDLER_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

ADD_EXECUTABLE(
        tests_handler 
        src/main.cpp
        src/handlers_btest.cpp
        src/predict_controller_btest.cpp
)
TARGET_LINK_LIBRARIES(tests_handler ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}  ${HANDLER_LIBRARIES} -lgmock -lstdc++fs)

SET(memcheck_command)

IF (WITH_MEMCHECK)
        MESSAGE("Memcheck enabled")
        SET(memcheck_command ${CMAKE_SOURCE_DIR}/memtest.sh)
ELSE()
        MESSAGE("Memcheck disabled")
ENDIF()

SET(TEST_COMMAND ${memcheck_command} ${CMAKE_BINARY_DIR}/project/handlerlib/tests/tests_handler)

ADD_TEST(
        NAME HandlerTest
        COMMAND ${TEST_COMMAND}
)