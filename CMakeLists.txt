cmake_minimum_required(VERSION 3.15)
project(SImProject)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES main.cpp RTC.c Time.c)
set(HEADER_FILES
        boolean.h
        RTC.h
        SimFiles/SimulationConfigurations.h
        Time.h
        )


set(SOURCE_FILES
        main.c
        RTC.c
        Time.c
        )

add_executable(SImProject
        main.c
        RTC.c
        Time.c
        SimFiles/GPS_Module.h SimFiles/GPS_Module.c SimFiles/SimSTK.c SimFiles/SimSTK.h SimFiles/GPS_types.h)