FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(include ${DB_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

ADD_EXECUTABLE(
        tests_db
        src/dbcontroller_test.cpp
        src/repositories_tests.cpp
        src/repositorycache_tests.cpp
)

TARGET_LINK_LIBRARIES(tests_db ${DB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES} GTest::gtest_main GTest::gmock_main  )

# TARGET_LINK_LIBRARIES(tests_db ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${DB_LIBRARIES} -lgmock -lstdc++fs)


SET(memcheck_command)

IF (WITH_MEMCHECK)
        MESSAGE("Memcheck enabled")
        SET(memcheck_command ${CMAKE_SOURCE_DIR}/memtest.sh)
ELSE()
        MESSAGE("Memcheck disabled")
ENDIF()

SET(TEST_COMMAND ${memcheck_command} ${CMAKE_BINARY_DIR}/project/dblib/tests/tests_db)

ADD_TEST(
        NAME DBTest
        COMMAND ${TEST_COMMAND}
)
