################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tcpsocket/Tcpconnector.cpp \
../src/tcpsocket/Tcpstream.cpp 

OBJS += \
./src/tcpsocket/Tcpconnector.o \
./src/tcpsocket/Tcpstream.o 

CPP_DEPS += \
./src/tcpsocket/Tcpconnector.d \
./src/tcpsocket/Tcpstream.d 


# Each subdirectory must supply rules for building sources it contributes
src/tcpsocket/%.o: ../src/tcpsocket/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


