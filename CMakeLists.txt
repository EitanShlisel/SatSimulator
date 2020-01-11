cmake_minimum_required(VERSION 3.15)
project(SImProject)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES main.cpp RTC.c Time.c)
set(HEADER_FILES
        SubsystemModules/boolean.h
        SubsystemModules/RTC.h
        SimFiles/SimulationConfigurations.h
        SubsystemModules/Time.h
        )


set(SOURCE_FILES
        main.c
        SubsystemModules/RTC.c
        SubsystemModules/Time.c
        )

add_executable(SImProject
        main.c
        SubsystemModules/RTC.c
        SubsystemModules/Time.c
        SubsystemModules/GPS_Module.h SubsystemModules/GPS_Module.c SimFiles/SimSTK.c SimFiles/SimSTK.h SubsystemModules/GPS_types.h SimFiles/SimRTC.c SimFiles/SimRTC.h SimFiles/TimeHelperFunctions.c SimFiles/TimeHelperFunctions.h SimFiles/SimI2C.c SimFiles/SimI2C.h SimFiles/SimEPS.c SimFiles/SimEPS.h SimFiles/GenericHelpFunctions.c SimFiles/GenericHelpFunctions.h)