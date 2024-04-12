set(SRC_LIST test_coroutine.cc)
enable_language(C ASM)
add_executable(test_coroutine ${SRC_LIST})
target_link_libraries(test_coroutine coroutine)


set(SRC_LIST test_coroutinepool.cc)
enable_language(C ASM)
add_executable(test_coroutinepool ${SRC_LIST})
target_link_libraries(test_coroutinepool coroutine)