################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/opencctv/ApplicationModel.cpp \
../src/opencctv/ConsumerThread.cpp \
../src/opencctv/Exception.cpp \
../src/opencctv/Image.cpp \
../src/opencctv/ImageMulticaster.cpp \
../src/opencctv/Process.cpp \
../src/opencctv/ProducerThread.cpp \
../src/opencctv/ResultRouterThread.cpp 

OBJS += \
./src/opencctv/ApplicationModel.o \
./src/opencctv/ConsumerThread.o \
./src/opencctv/Exception.o \
./src/opencctv/Image.o \
./src/opencctv/ImageMulticaster.o \
./src/opencctv/Process.o \
./src/opencctv/ProducerThread.o \
./src/opencctv/ResultRouterThread.o 

CPP_DEPS += \
./src/opencctv/ApplicationModel.d \
./src/opencctv/ConsumerThread.d \
./src/opencctv/Exception.d \
./src/opencctv/Image.d \
./src/opencctv/ImageMulticaster.d \
./src/opencctv/Process.d \
./src/opencctv/ProducerThread.d \
./src/opencctv/ResultRouterThread.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/%.o: ../src/opencctv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


