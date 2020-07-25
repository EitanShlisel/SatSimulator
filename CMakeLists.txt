cmake_minimum_required(VERSION 3.15)
project(SImProject)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES main.cpp RTC.c Time.c)
set(HEADER_FILES
        SubsystemModules/boolean.h
        SubsystemModules/RTC.h
        SimFiles/SimConfigFiles/SimulationConfigurations.h
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
        SubsystemModules/GPS_Module.h
        SubsystemModules/GPS_Module.c
        SubsystemModules/FRAM.c
        SubsystemModules/FRAM.h
        SimFiles/SimSTK.c SimFiles/SimSTK.h
        SubsystemModules/GPS_types.h
        SimFiles/SimRTC.c SimFiles/SimRTC.h
        Helper/TimeHelperFunctions.c Helper/TimeHelperFunctions.h
        SimFiles/SimI2C.c SimFiles/SimI2C.h
        SimFiles/SimEPS.c SimFiles/SimEPS.h
        Helper/GenericHelpFunctions.c
        Helper/GenericHelpFunctions.h
        SimFiles/SimSolar.c
        SimFiles/SimSolar.h
        SimFiles/SimConfigFiles/SimConfigEPS.h
        SimFiles/SimConfigFiles/SimConfigGPS.h
        SimFiles/SimConfigFiles/SimConfigI2C.h
        SimFiles/SimConfigFiles/SimConfigSolar.h
        SimFiles/SimConfigFiles/SimConfigTRX.h
        SimFiles/SimTRX.c
        SimFiles/SimTRX.h
        SimFiles/SimConfigFiles/SimConfigThermodynamics.h
        SimFiles/SimThermodynamics.c
        SimFiles/SimThermodynamics.h
        Helper/List.c
        Helper/List.h
        SubsystemModules/isisTRXVU.c
        SubsystemModules/isisTRXVU.h
        SubsystemModules/I2C.c
        SubsystemModules/I2C.h
        SubsystemModules/hal/errors.h
        Helper/RunPython.c
        Helper/RunPython.h
        Helper/TcpHandler.c
        Helper/TcpHandler.h
        Helper/TcpPlotter.c
        Helper/TcpPlotter.h
        ConsumptionStates/ConsumptionStates.h ConsumptionStates/ConsumptionStates.c SimulatorCommandLine/SimCmdLine.c SimulatorCommandLine/SimCmdLine.h SimulatorCommandLine/CommandDictionary.h SimulatorCommandLine/CommandDictionary.c SimulatorCommandLine/CmdTypes.h Helper/threads.h Helper/threads.c Helper/sem.c Helper/sem.h)

if(MINGW)
    target_link_libraries(SImProject ws2_32)
endif()