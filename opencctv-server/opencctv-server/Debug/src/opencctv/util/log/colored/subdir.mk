################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/util/log/colored/ColorLogger.cpp 

OBJS += \
./src/opencctv/util/log/colored/ColorLogger.o 

CPP_DEPS += \
./src/opencctv/util/log/colored/ColorLogger.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/util/log/colored/%.o: ../src/opencctv/util/log/colored/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

