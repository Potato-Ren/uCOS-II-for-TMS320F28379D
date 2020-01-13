################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
uCOS-II/Source/%.obj: ../uCOS-II/Source/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII/uC-CPU" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII/uC-LIB" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII/OSConfig" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII/uCOS-II/Source" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII/uCOS-II/Ports/C28x/Generic/CCS" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/ucosII/uC-CPU/C28x/CCS" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/F2837xD_headers/include" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/F2837xD_common/include" --include_path="C:/Users/rren3/workspace_v8/ucosII_CCS/F2837xD_common" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.4.LTS/include" --advice:performance=all --define=CPU1 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="uCOS-II/Source/$(basename $(<F)).d_raw" --obj_directory="uCOS-II/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


