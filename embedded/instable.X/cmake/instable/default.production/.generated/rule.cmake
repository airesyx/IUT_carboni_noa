# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "D:/Tools/IDE/VSCode-win32-x64-1.136.1/data/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(instable_default_default_XC16_assemble_rule target)
    set(options
        "-g"
        "-mcpu=33EP512MU814"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUGGER_ICD4=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_ICD4=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endfunction()
function(instable_default_default_XC16_assemblePreproc_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "-mcpu=33EP512MU814"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUGGER_ICD4=1,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_ICD4=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endfunction()
function(instable_default_default_XC16_compile_rule target)
    set(options
        "-g"
        "-mcpu=33EP512MU814"
        "-O0"
        "-msmart-io=1"
        "-Wall"
        "-msfr-warn=off"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_ICD4=1"
        PRIVATE "XPRJ_default=default")
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
endfunction()
function(instable_default_dependentObject_rule target)
    set(options
        "-c"
        "-mcpu=33EP512MU814"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(instable_default_link_rule target)
    set(options
        "-g"
        "-mcpu=33EP512MU814"
        "-Wl,--script=p33EP512MU814.gld,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUGGER_ICD4=1,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path=${CMAKE_CURRENT_SOURCE_DIR}/../../..,--no-force-link,--smart-io,--report-mem,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__MPLAB_DEBUGGER_ICD4=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(instable_default_bin2hex_rule target)
    add_custom_target(
        instable_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${instable_default_image_name} -a -mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16
        WORKING_DIRECTORY ${instable_default_output_dir}
        BYPRODUCTS "${instable_default_output_dir}/${instable_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(instable_default_Bin2Hex ${target})
endfunction()
function(instable_default_objcopy_lss_rule target)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${OBJDUMP}
        ARGS --disassemble --wide --demangle --line-numbers --section-headers --source ${instable_default_image_name} > ${instable_default_image_base_name}.lss
        WORKING_DIRECTORY ${instable_default_output_dir})
endfunction()
