################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/test/gateway/TestAnalyticInstanceStreamGateway.cpp \
../src/test/gateway/TestStreamGateway.cpp 

OBJS += \
./src/test/gateway/TestAnalyticInstanceStreamGateway.o \
./src/test/gateway/TestStreamGateway.o 

CPP_DEPS += \
./src/test/gateway/TestAnalyticInstanceStreamGateway.d \
./src/test/gateway/TestStreamGateway.d 


# Each subdirectory must supply rules for building sources it contributes
src/test/gateway/%.o: ../src/test/gateway/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


