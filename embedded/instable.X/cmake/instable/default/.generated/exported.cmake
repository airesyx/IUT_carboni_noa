set(DEPENDENT_MP_BIN2HEXinstable_default_kB3zB907 "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFinstable_default_kB3zB907 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/instable/default.elf)
set(DEPENDENT_TARGET_DIRinstable_default_kB3zB907 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/instable)
set(DEPENDENT_BYPRODUCTSinstable_default_kB3zB907 ${DEPENDENT_TARGET_DIRinstable_default_kB3zB907}/${sourceFileNameinstable_default_kB3zB907}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRinstable_default_kB3zB907}/${sourceFileNameinstable_default_kB3zB907}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXinstable_default_kB3zB907} ${DEPENDENT_DEPENDENT_TARGET_ELFinstable_default_kB3zB907} --image ${sourceFileNameinstable_default_kB3zB907} ${addressinstable_default_kB3zB907} ${modeinstable_default_kB3zB907} -mdfp=D:/Tools/IDE/VSCode-win32-x64-1.136.1/data/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRinstable_default_kB3zB907}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFinstable_default_kB3zB907})
add_custom_target(
    dependent_produced_source_artifactinstable_default_kB3zB907 
    DEPENDS ${DEPENDENT_TARGET_DIRinstable_default_kB3zB907}/${sourceFileNameinstable_default_kB3zB907}.s
    )
