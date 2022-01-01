set (SHADER_BUILD_NAME Shaders)

project(${SHADER_BUILD_NAME})

message("Building ${SHADER_BUILD_NAME}...")

file(GLOB FS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/default" "default/*.sc") 
file(GLOB VS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/default" "default/*.sc") 

list(FILTER FS_FILES INCLUDE REGEX "^fs.*\\.sc$") 
list(FILTER VS_FILES INCLUDE REGEX "^vs.*\\.sc$")

add_custom_target(run ALL)

foreach(file ${FS_FILES})
    if(WIN32)
        # shaderc(FILE "default/${file}" OUTPUT "default/${file}.bin" FRAGMENT WINDOWS)

        message("\"tools/shaderc_win.exe\" -i \"tools/include\" -p ps_4_0 -O 3 --type fragment --platform windows -f \"default/${file}\" -o \"default/${file}.bin\" --varyingdef \"default/varying.def.sc\"")

        add_custom_command (
            OUTPUT "default/${file}.bin"
            COMMAND "tools/shaderc_win.exe"
            -i "tools/include"
            -p ps_4_0 -O 3
            --type fragment 
            --platform "windows"
            -f "default/${file}" 
            -o "default/${file}.bin"
            --varyingdef "default/varying.def.sc"
            DEPENDS "default/${file}" "tools/shaderc_win.exe"
        )
    endif()
endforeach()

foreach(file ${VS_FILES})    
    if(WIN32)
        # shaderc(FILE "default/${file}" OUTPUT "default/${file}.bin" VERTEX WINDOWS)

        message("\"tools/shaderc_win.exe\" -i \"tools/include\" -p vs_4_0 -O 3 --type vertex --platform windows -f \"default/${file}\" -o \"default/${file}.bin\" --varyingdef \"default/varying.def.sc\"")
        add_custom_command(
            OUTPUT "default/${file}.bin"
            COMMAND "tools/shaderc_win.exe"
            -i "tools/include"
            -p vs_4_0 -O 3
            --type vertex 
            --platform "windows"
            -f "default/${file}" 
            -o "default/${file}.bin"
            --varyingdef "default/varying.def.sc"
            DEPENDS "default/${file}" "tools/shaderc_win.exe"
        )
    endif()
endforeach()
