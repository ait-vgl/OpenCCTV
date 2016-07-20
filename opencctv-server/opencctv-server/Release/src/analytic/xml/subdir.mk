################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/analytic/xml/AnalyticMessage.cpp 

OBJS += \
./src/analytic/xml/AnalyticMessage.o 

CPP_DEPS += \
./src/analytic/xml/AnalyticMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/analytic/xml/%.o: ../src/analytic/xml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


