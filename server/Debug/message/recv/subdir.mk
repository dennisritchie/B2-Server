################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../message/recv/recv_twins.c 

OBJS += \
./message/recv/recv_twins.o 

C_DEPS += \
./message/recv/recv_twins.d 


# Each subdirectory must supply rules for building sources it contributes
message/recv/%.o: ../message/recv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


