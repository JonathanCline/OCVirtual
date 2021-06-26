cmake_minimum_required(VERSION 3.8)

project(OCVirtual)

macro(GET_LIB_NAMESPACE outvar)
	set(${outvar} ocv)
endmacro(GET_LIB_NAMESPACE )

set(lib_namespace )
GET_LIB_NAMESPACE(lib_namespace)

add_library(${PROJECT_NAME} STATIC "include/${PROJECT_NAME}.h" "source/${PROJECT_NAME}.cpp")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} PUBLIC liblua ${PROJECT_NAME}-support)
target_include_directories(${PROJECT_NAME} PUBLIC "include" PRIVATE "source")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAMESPACE=${lib_namespace})

add_executable(${PROJECT_NAME}-cli "main.cpp")
target_link_libraries(${PROJECT_NAME}-cli PUBLIC ${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME}-cli PRIVATE
	PROJECT_NAMESPACE=${lib_namespace}
	SOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/") 

add_subdirectory("lib")

include("CMakeUtils.cmake")
add_subdirectory("source")

install(TARGETS ${PROJECT_NAME} DESTINATION "lib")
install(TARGETS ${PROJECT_NAME}-cli DESTINATION "bin")
install(FILES "include/${PROJECT_NAME}.h" DESTINATION "include")
