set(SRC_LIST test_coroutine.cc)
enable_language(C ASM)
add_executable(test_coroutine ${SRC_LIST})
target_link_libraries(test_coroutine coroutine)