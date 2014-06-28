################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../socket/recv/recv.c 

OBJS += \
./socket/recv/recv.o 

C_DEPS += \
./socket/recv/recv.d 


# Each subdirectory must supply rules for building sources it contributes
socket/recv/%.o: ../socket/recv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


