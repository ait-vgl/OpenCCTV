################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/analytic/util/log/colored/ColorLogger.cpp 

OBJS += \
./src/analytic/util/log/colored/ColorLogger.o 

CPP_DEPS += \
./src/analytic/util/log/colored/ColorLogger.d 


# Each subdirectory must supply rules for building sources it contributes
src/analytic/util/log/colored/%.o: ../src/analytic/util/log/colored/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


