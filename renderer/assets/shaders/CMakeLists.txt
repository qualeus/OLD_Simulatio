set (SHADER_BUILD_NAME Shaders)

project(${SHADER_BUILD_NAME})

message("\nBuilding ${SHADER_BUILD_NAME}...")

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

foreach (directory ${DIRECTORIES})

    file(GLOB FS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${directory}" "${directory}/*.sc") 
    file(GLOB VS_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${directory}" "${directory}/*.sc") 

    list(FILTER FS_FILES INCLUDE REGEX "^fs.*\\.sc$") 
    list(FILTER VS_FILES INCLUDE REGEX "^vs.*\\.sc$")

    if(FS_FILES)
        message("\nBuilding Shaders for repertory ${directory}...")
    endif()

    foreach(file ${FS_FILES})
        if(WIN32)
            get_filename_component(bare_file ${file} NAME_WLE)

            message("\"tools/shaderc_win.exe\" -i \"tools/include\" -p ps_5_0 -O 3 --type fragment --platform windows -f \"${directory}/${file}\" -o \"${directory}/${bare_file}.bin.hpp\" --varyingdef \"${directory}/varying.def.sc\" --bin2c \"${bare_file}\"")

            execute_process(
                COMMAND "tools/shaderc_win.exe"
                -i "tools/include"
                -p ps_5_0 -O 3
                --type fragment 
                --platform "windows"
                -f "${directory}/${file}" 
                -o "${directory}/${bare_file}.bin.hpp"
                --varyingdef "${directory}/varying.def.sc"
                --bin2c "${bare_file}_dx11"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endif()
    endforeach()

    foreach(file ${VS_FILES})    
        if(WIN32)
            get_filename_component(bare_file ${file} NAME_WLE)

            message("\"tools/shaderc_win.exe\" -i \"tools/include\" -p vs_5_0 -O 3 --type vertex --platform windows -f \"${directory}/${file}\" -o \"${directory}/${bare_file}.bin.hpp\" --varyingdef \"${directory}/varying.def.sc\" --bin2c \"${bare_file}\"")

            execute_process(
                COMMAND "tools/shaderc_win.exe"
                -i "tools/include"
                -p vs_5_0 -O 3
                --type vertex 
                --platform "windows"
                -f "${directory}/${file}" 
                -o "${directory}/${bare_file}.bin.hpp"
                --varyingdef "${directory}/varying.def.sc"
                --bin2c "${bare_file}_dx11"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endif()
    endforeach()

endforeach()

add_custom_target(${SHADER_BUILD_NAME}_BUILDS ALL DEPENDS ${BUILDED_SHADERS})
# add_dependencies(${SHADER_BUILD_NAME} ${SHADER_BUILD_NAME}_BUILDS)

message("\n${SHADER_BUILD_NAME} have been succefully builded.\n")
