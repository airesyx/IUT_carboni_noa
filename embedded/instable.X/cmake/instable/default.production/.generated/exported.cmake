set(DEPENDENT_MP_BIN2HEXinstable_default_7SmSGJZp "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFinstable_default_7SmSGJZp ${CMAKE_CURRENT_LIST_DIR}/../../../../out/instable/production/default-production.elf)
set(DEPENDENT_TARGET_DIRinstable_default_7SmSGJZp ${CMAKE_CURRENT_LIST_DIR}/../../../../out/instable/production)
set(DEPENDENT_BYPRODUCTSinstable_default_7SmSGJZp ${DEPENDENT_TARGET_DIRinstable_default_7SmSGJZp}/${sourceFileNameinstable_default_7SmSGJZp}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRinstable_default_7SmSGJZp}/${sourceFileNameinstable_default_7SmSGJZp}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXinstable_default_7SmSGJZp} ${DEPENDENT_DEPENDENT_TARGET_ELFinstable_default_7SmSGJZp} --image ${sourceFileNameinstable_default_7SmSGJZp} ${addressinstable_default_7SmSGJZp} ${modeinstable_default_7SmSGJZp} -mdfp=D:/Tools/IDE/VSCode-win32-x64-1.136.1/data/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRinstable_default_7SmSGJZp}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFinstable_default_7SmSGJZp})
add_custom_target(
    dependent_produced_source_artifactinstable_default_7SmSGJZp 
    DEPENDS ${DEPENDENT_TARGET_DIRinstable_default_7SmSGJZp}/${sourceFileNameinstable_default_7SmSGJZp}.s
    )
