enable_language(CXX)

file(GLOB TESTS_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)
add_executable(tests ${TESTS_FILES})
set_target_properties(tests PROPERTIES LINKER_LANGUAGE "CXX")

add_dependencies(tests deque)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests deque)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    if(${CMAKE_C_COMPILER_ID} MATCHES "(Apple)?[Cc]lang")
        target_compile_options(tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    elseif(${CMAKE_C_COMPILER_ID} MATCHES "GNU")
        target_compile_options(tests PRIVATE --coverage)
        target_link_options(tests PRIVATE --coverage)
    endif()
endif()
