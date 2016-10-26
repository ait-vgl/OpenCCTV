################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/opencctv/util/serialization/gpb/analytic_result.pb.cc \
../src/opencctv/util/serialization/gpb/image.pb.cc 

CPP_SRCS += \
../src/opencctv/util/serialization/gpb/ProtoBuf.cpp 

CC_DEPS += \
./src/opencctv/util/serialization/gpb/analytic_result.pb.d \
./src/opencctv/util/serialization/gpb/image.pb.d 

OBJS += \
./src/opencctv/util/serialization/gpb/ProtoBuf.o \
./src/opencctv/util/serialization/gpb/analytic_result.pb.o \
./src/opencctv/util/serialization/gpb/image.pb.o 

CPP_DEPS += \
./src/opencctv/util/serialization/gpb/ProtoBuf.d 


# Each subdirectory must supply rules for building sources it contributes
src/opencctv/util/serialization/gpb/%.o: ../src/opencctv/util/serialization/gpb/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/opencctv/util/serialization/gpb/%.o: ../src/opencctv/util/serialization/gpb/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


