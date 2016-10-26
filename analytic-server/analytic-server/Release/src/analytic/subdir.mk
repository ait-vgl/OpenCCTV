################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/analytic/AnalyticResult.cpp \
../src/analytic/ConsumerThread.cpp \
../src/analytic/ProducerThread.cpp 

OBJS += \
./src/analytic/AnalyticResult.o \
./src/analytic/ConsumerThread.o \
./src/analytic/ProducerThread.o 

CPP_DEPS += \
./src/analytic/AnalyticResult.d \
./src/analytic/ConsumerThread.d \
./src/analytic/ProducerThread.d 


# Each subdirectory must supply rules for building sources it contributes
src/analytic/%.o: ../src/analytic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


