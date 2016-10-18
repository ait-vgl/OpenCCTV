################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/analytic/AnalyticData.cpp \
../src/analytic/AnalyticResult.cpp \
../src/analytic/AnalyticServer.cpp \
../src/analytic/AnalyticServerManager.cpp 

OBJS += \
./src/analytic/AnalyticData.o \
./src/analytic/AnalyticResult.o \
./src/analytic/AnalyticServer.o \
./src/analytic/AnalyticServerManager.o 

CPP_DEPS += \
./src/analytic/AnalyticData.d \
./src/analytic/AnalyticResult.d \
./src/analytic/AnalyticServer.d \
./src/analytic/AnalyticServerManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/analytic/%.o: ../src/analytic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


