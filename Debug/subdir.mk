################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJ_SRCS += \
../doll.obj \
../obj.obj \
../scene.obj \
../simple.obj 

CPP_SRCS += \
../group.cpp \
../main.cpp \
../objectItem.cpp 

OBJS += \
./group.o \
./main.o \
./objectItem.o 

CPP_DEPS += \
./group.d \
./main.d \
./objectItem.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


