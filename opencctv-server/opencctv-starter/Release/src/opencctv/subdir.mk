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
../src/opencctv/OpenCCTVServer.cpp \
../src/opencctv/OpenCCTVServerManager.cpp \
../src/opencctv/Process.cpp \
../src/opencctv/ProducerThread.cpp \
../src/opencctv/ResultRouterThread.cpp \
../src/opencctv/ServerController.cpp \
../src/opencctv/StreamData.cpp \
../src/opencctv/ThreadPool.cpp \
../src/opencctv/UnitTest.cpp \
../src/opencctv/UnitTest_test.cpp 

OBJS += \
./src/opencctv/ApplicationModel.o \
./src/opencctv/ConsumerThread.o \
./src/opencctv/Exception.o \
./src/opencctv/Image.o \
./src/opencctv/ImageMulticaster.o \
./src/opencctv/OpenCCTVServer.o \
./src/opencctv/OpenCCTVServerManager.o \
./src/opencctv/Process.o \
./src/opencctv/ProducerThread.o \
./src/opencctv/ResultRouterThread.o \
./src/opencctv/ServerController.o \
./src/opencctv/StreamData.o \
./src/opencctv/ThreadPool.o \
./src/opencctv/UnitTest.o \
./src/opencctv/UnitTest_test.o 

CPP_DEPS += \
./src/opencctv/ApplicationModel.d \
./src/opencctv/ConsumerThread.d \
./src/opencctv/Exception.d \
./src/opencctv/Image.d \
./src/opencctv/ImageMulticaster.d \
./src/opencctv/OpenCCTVServer.d \
./src/opencctv/OpenCCTVServerManager.d \
./src/opencctv/Process.d \
./src/opencctv/ProducerThread.d \
./src/opencctv/ResultRouterThread.d \
./src/opencctv/ServerController.d \
./src/opencctv/StreamData.d \
./src/opencctv/ThreadPool.d \
./src/opencctv/UnitTest.d \
./src/opencctv/UnitTest_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/%.o: ../src/opencctv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


