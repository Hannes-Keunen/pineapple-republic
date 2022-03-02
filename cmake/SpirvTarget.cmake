function(add_spirv_target)
    set(TARGET_NAME ${ARGV0})
    add_library(${TARGET_NAME} INTERFACE)
    list(POP_FRONT ARGN)
    foreach(SHADER ${ARGN})
        get_filename_component(SHADER_NAME ${SHADER} NAME_WLE)
        set(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv)
        set(SHADER_TARGET ${TARGET_NAME}-${SHADER_NAME})

        add_custom_target(${SHADER_TARGET} DEPENDS ${OUTPUT})
        add_custom_command(
            OUTPUT ${OUTPUT}
            COMMAND glslangValidator -G -o ${OUTPUT} ${SHADER}
            DEPENDS ${SHADER}
            VERBATIM)
        add_dependencies(${TARGET_NAME} ${SHADER_TARGET})
    endforeach()
endfunction(add_spirv_target)