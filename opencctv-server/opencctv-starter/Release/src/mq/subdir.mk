################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mq/TcpMq.cpp 

OBJS += \
./src/mq/TcpMq.o 

CPP_DEPS += \
./src/mq/TcpMq.d 


# Each subdirectory must supply rules for building sources it contributes
src/mq/%.o: ../src/mq/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


