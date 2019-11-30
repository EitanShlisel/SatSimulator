cmake_minimum_required(VERSION 3.15)
project(SImProject)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES main.cpp RTC.c Time.c)
file(GLOB SOURCES
        *.h
        *.c
        )
#add_executable(SImProject cmake-build-debug/main.cpp cmake-build-debug/RTC.c cmake-build-debug/RTC.h cmake-build-debug/SimulationConfigurations.h cmake-build-debug/Time.c cmake-build-debug/Time.h cmake-build-debug/boolean.h)
add_executable(SImProject
        main.c
        RTC.c
        Time.c
        SimulationConfigurations.h
        RTC.h
        Time.h
        boolean.h
        )