################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/analytic/AnalyticData.cpp \
../src/analytic/AnalyticInstanceManager.cpp \
../src/analytic/AnalyticResult.cpp 

OBJS += \
./src/analytic/AnalyticData.o \
./src/analytic/AnalyticInstanceManager.o \
./src/analytic/AnalyticResult.o 

CPP_DEPS += \
./src/analytic/AnalyticData.d \
./src/analytic/AnalyticInstanceManager.d \
./src/analytic/AnalyticResult.d 


# Each subdirectory must supply rules for building sources it contributes
src/analytic/%.o: ../src/analytic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


