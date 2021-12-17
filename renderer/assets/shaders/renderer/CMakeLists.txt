set (SHADER_BUILD_NAME Shaders)

project(${SHADER_BUILD_NAME})

message("Building ${SHADER_BUILD_NAME}...")

file(GLOB FS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.sc") 
file(GLOB VS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.sc") 

list(FILTER FS_FILES INCLUDE REGEX "^fs.*\\.sc$") 
list(FILTER VS_FILES INCLUDE REGEX "^vs.*\\.sc$")

message(${VS_FILES})

foreach(file ${FS_FILES})
    message(${file} => ${file}.bin)
    add_custom_target(${SHADER_BUILD_NAME}_Fragment ALL shaderc -f ../renderer/${file} -o ../renderer/${file}.bin --type f WORKING_DIRECTORY ../shaderc)
endforeach()

foreach(file ${VS_FILES})
    message(${file} => ${file}.bin)
    add_custom_target(${SHADER_BUILD_NAME}_Vertex ALL shaderc -f ../renderer/${file} -o ../renderer/${file}.bin --type v WORKING_DIRECTORY ../shaderc)
endforeach()
