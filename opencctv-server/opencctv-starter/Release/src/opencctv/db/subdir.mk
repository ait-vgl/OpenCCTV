################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/db/AnalyticInstanceStreamGateway.cpp \
../src/opencctv/db/AnalyticResultGateway.cpp \
../src/opencctv/db/DbConnector.cpp \
../src/opencctv/db/StreamGateway.cpp 

OBJS += \
./src/opencctv/db/AnalyticInstanceStreamGateway.o \
./src/opencctv/db/AnalyticResultGateway.o \
./src/opencctv/db/DbConnector.o \
./src/opencctv/db/StreamGateway.o 

CPP_DEPS += \
./src/opencctv/db/AnalyticInstanceStreamGateway.d \
./src/opencctv/db/AnalyticResultGateway.d \
./src/opencctv/db/DbConnector.d \
./src/opencctv/db/StreamGateway.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/db/%.o: ../src/opencctv/db/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


