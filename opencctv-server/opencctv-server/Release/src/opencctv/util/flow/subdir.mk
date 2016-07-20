################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/util/flow/BasicFlowController.cpp \
../src/opencctv/util/flow/SimpleFlowController.cpp 

OBJS += \
./src/opencctv/util/flow/BasicFlowController.o \
./src/opencctv/util/flow/SimpleFlowController.o 

CPP_DEPS += \
./src/opencctv/util/flow/BasicFlowController.d \
./src/opencctv/util/flow/SimpleFlowController.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/util/flow/%.o: ../src/opencctv/util/flow/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


