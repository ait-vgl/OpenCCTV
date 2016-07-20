################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/util/xml/OpenCCTVServerMessage.cpp 

OBJS += \
./src/opencctv/util/xml/OpenCCTVServerMessage.o 

CPP_DEPS += \
./src/opencctv/util/xml/OpenCCTVServerMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/util/xml/%.o: ../src/opencctv/util/xml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


