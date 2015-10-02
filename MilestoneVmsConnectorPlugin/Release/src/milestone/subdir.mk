################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/milestone/MilestoneMessage.cpp \
../src/milestone/TcpMpegDecoder.cpp 

OBJS += \
./src/milestone/MilestoneMessage.o \
./src/milestone/TcpMpegDecoder.o 

CPP_DEPS += \
./src/milestone/MilestoneMessage.d \
./src/milestone/TcpMpegDecoder.d 


# Each subdirectory must supply rules for building sources it contributes
src/milestone/%.o: ../src/milestone/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


