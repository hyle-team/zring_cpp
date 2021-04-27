file(GLOB_RECURSE FUNC_TESTS functional_tests/*)

source_group(functional_tests FILES ${FUNC_TESTS})

add_executable(functional_tests ${FUNC_TESTS})

target_link_libraries(functional_tests crypto ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

set_property(TARGET functional_tests PROPERTY FOLDER "tests")
