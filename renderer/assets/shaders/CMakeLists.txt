set (SHADER_BUILD_NAME Shaders)

project(${SHADER_BUILD_NAME})

message("Building ${SHADER_BUILD_NAME}...")

file(GLOB FS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/default" "*.sc") 
file(GLOB VS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/default" "*.sc") 

list(FILTER FS_FILES INCLUDE REGEX "^fs.*\\.sc$") 
list(FILTER VS_FILES INCLUDE REGEX "^vs.*\\.sc$")

foreach(file ${FS_FILES})
    if(WIN32)
        message(shaderc(FILE "default/${file}" OUTPUT "default/${file}.bin" FRAGMENT WINDOWS))
        shaderc(FILE "default/${file}" OUTPUT "default/${file}.bin" FRAGMENT WINDOWS)
    endif()
endforeach()

foreach(file ${VS_FILES})    
    if(WIN32)
        message(shaderc(FILE "default/${file}" OUTPUT "default/${file}.bin" VERTEX WINDOWS))
        shaderc(FILE "default/${file}" OUTPUT "default/${file}.bin" VERTEX WINDOWS)
    endif()
endforeach()
