################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/analytic/AnalyticProcess.cpp \
../src/analytic/ApplicationModel.cpp 

OBJS += \
./src/analytic/AnalyticProcess.o \
./src/analytic/ApplicationModel.o 

CPP_DEPS += \
./src/analytic/AnalyticProcess.d \
./src/analytic/ApplicationModel.d 


# Each subdirectory must supply rules for building sources it contributes
src/analytic/%.o: ../src/analytic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


