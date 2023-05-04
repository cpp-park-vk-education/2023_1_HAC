FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

#add_compile_options(-lssl -lcrypto)

FILE(GLOB SOURCES_TEST ../src/*)
INCLUDE_DIRECTORIES(include ${SERVERLIB_INCLUDE_DIRS} ${HANDLER_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

ADD_EXECUTABLE(
        tests_server 
        src/main.cpp
        src/server_btest.cpp
        src/api_model_btest.cpp
        src/api_stock_btest.cpp
        src/http_protocol_btest.cpp
        src/listener_btest.cpp
        src/routers_btest.cpp
        src/session_btest.cpp
        ${SOURCES_TEST}
        #../src/http_protocol.cpp
)
TARGET_LINK_LIBRARIES(tests_server ${GTEST_BOTH_LIBRARIES} ${HANDLER_INCLUDE_DIRS} ${SERVERLIB_INCLUDE_DIRS}${DB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lgmock -lstdc++fs)


SET(memcheck_command)

IF (WITH_MEMCHECK)
        MESSAGE("Memcheck enabled")
        SET(memcheck_command ${CMAKE_SOURCE_DIR}/memtest.sh)
ELSE()
        MESSAGE("Memcheck disabled")
ENDIF()

SET(TEST_COMMAND ${memcheck_command} ${CMAKE_BINARY_DIR}/project/serverlib/tests/tests_server)

ADD_TEST(
        NAME NetTest
        COMMAND ${TEST_COMMAND}
)