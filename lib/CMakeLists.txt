add_subdirectory("lua")

add_library(${PROJECT_NAME}-support INTERFACE)
target_include_directories(${PROJECT_NAME}-support INTERFACE "support")
target_compile_features(${PROJECT_NAME}-support INTERFACE cxx_std_20)
