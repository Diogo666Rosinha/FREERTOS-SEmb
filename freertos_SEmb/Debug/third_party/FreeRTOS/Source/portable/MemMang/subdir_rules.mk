################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
third_party/FreeRTOS/Source/portable/MemMang/heap_2.obj: C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS/Source/portable/MemMang/heap_2.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-arm_20.2.6.LTS/include" --include_path="C:/Users/diogo/workspace_v11/Embebidos/Debug/freertos_SEmb" --include_path="C:/ti/tivaware_c_series_2_1_4_178/examples/boards/ek-tm4c123gxl" --include_path="C:/ti/tivaware_c_series_2_1_4_178" --include_path="C:/ti/tivaware_c_series_2_1_4_178/third_party" --include_path="C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS/Source/include" --include_path="C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS" --include_path="C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Source/portable/MemMang/$(basename $(<F)).d_raw" --obj_directory="third_party/FreeRTOS/Source/portable/MemMang" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

