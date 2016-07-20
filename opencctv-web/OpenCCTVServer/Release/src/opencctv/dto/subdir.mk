################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/dto/AnalyticInstanceStream.cpp \
../src/opencctv/dto/AnalyticResult.cpp \
../src/opencctv/dto/Stream.cpp 

OBJS += \
./src/opencctv/dto/AnalyticInstanceStream.o \
./src/opencctv/dto/AnalyticResult.o \
./src/opencctv/dto/Stream.o 

CPP_DEPS += \
./src/opencctv/dto/AnalyticInstanceStream.d \
./src/opencctv/dto/AnalyticResult.d \
./src/opencctv/dto/Stream.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/dto/%.o: ../src/opencctv/dto/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


