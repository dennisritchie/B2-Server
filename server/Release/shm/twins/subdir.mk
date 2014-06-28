################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../shm/twins/shm_twins_recv.c \
../shm/twins/shm_twins_send.c 

OBJS += \
./shm/twins/shm_twins_recv.o \
./shm/twins/shm_twins_send.o 

C_DEPS += \
./shm/twins/shm_twins_recv.d \
./shm/twins/shm_twins_send.d 


# Each subdirectory must supply rules for building sources it contributes
shm/twins/%.o: ../shm/twins/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


