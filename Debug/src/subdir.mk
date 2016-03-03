################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/moc_GLWidget.cpp \
../src/moc_QTMainWindow.cpp \
../src/moc_QTWindow.cpp 

OBJS += \
./src/main.o \
./src/moc_GLWidget.o \
./src/moc_QTMainWindow.o \
./src/moc_QTWindow.o 

CPP_DEPS += \
./src/main.d \
./src/moc_GLWidget.d \
./src/moc_QTMainWindow.d \
./src/moc_QTWindow.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DQT_NO_DEBUG -DGL_GLEXT_PROTOTYPES -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/share/qt4/mkspecs/linux-g++ -I"/home/lionis/workspace.doutorado/VMTK_QT/src/gl" -I"/home/lionis/workspace.doutorado/VMTK_QT/src/gui" -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -I/usr/X11R6/include -I"/home/lionis/workspace.doutorado/VMTK_QT/src/io" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


