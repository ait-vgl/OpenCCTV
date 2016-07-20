################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/zoneminder/HttpClient.cpp \
../src/zoneminder/MpegDecoder.cpp \
../src/zoneminder/ZoneminderMessage.cpp 

OBJS += \
./src/zoneminder/HttpClient.o \
./src/zoneminder/MpegDecoder.o \
./src/zoneminder/ZoneminderMessage.o 

CPP_DEPS += \
./src/zoneminder/HttpClient.d \
./src/zoneminder/MpegDecoder.d \
./src/zoneminder/ZoneminderMessage.d 


# Each subdirectory must supply rules for building sources it contributes
src/zoneminder/%.o: ../src/zoneminder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


