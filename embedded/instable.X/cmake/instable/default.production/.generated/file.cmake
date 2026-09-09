# The following variables contains the files used by the different stages of the build process.
set(instable_default_default_XC16_FILE_TYPE_assemble)
set_source_files_properties(${instable_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${instable_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(instable_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${instable_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${instable_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(instable_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ChipConfig.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../IO.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PWM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Robot.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Toolbox.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../timer.c")
set_source_files_properties(${instable_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(instable_default_default_XC16_FILE_TYPE_link)
set(instable_default_default_XC16_FILE_TYPE_bin2hex)
set(instable_default_default_XC16_FILE_TYPE_objcopy_lss)
set(instable_default_image_name "default-production.elf")
set(instable_default_image_base_name "default-production")

# The output directory of the final image.
set(instable_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/instable/production")

# The full path to the final image.
set(instable_default_full_path_to_image ${instable_default_output_dir}/${instable_default_image_name})
