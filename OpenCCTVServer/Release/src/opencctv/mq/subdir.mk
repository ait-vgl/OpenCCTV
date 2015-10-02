################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/mq/MqUtil.cpp \
../src/opencctv/mq/TcpMqReceiver.cpp \
../src/opencctv/mq/TcpMqSender.cpp 

OBJS += \
./src/opencctv/mq/MqUtil.o \
./src/opencctv/mq/TcpMqReceiver.o \
./src/opencctv/mq/TcpMqSender.o 

CPP_DEPS += \
./src/opencctv/mq/MqUtil.d \
./src/opencctv/mq/TcpMqReceiver.d \
./src/opencctv/mq/TcpMqSender.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/mq/%.o: ../src/opencctv/mq/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


