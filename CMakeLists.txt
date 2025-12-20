cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

# Set the project name
set(CMAKE_PROJECT_NAME GuardianElf)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Utils/Src/USART2.c
        Utils/Src/USART1.c
        Libs/GP10/GP10_data_struct.h
        Libs/GP10/GP10.h
        Libs/GP10/GP10.c
        Libs/OLCD/OLED.c
        Libs/OLCD/OLED_Data.c
        Application/app.c
        Application/app.h
        Utils/Src/USART3.c
        Utils/Inc/USART3.h
        Application/Src/OLED_Service.c
        Application/Inc/OLED_Service.h
        Libs/OLCD/OLED_Menu.c
        Libs/OLCD/OLED_Menu.h
        Application/Src/USART_Service.c
        Application/Inc/USART_Service.h
        Utils/Src/I2C.c
        Utils/Inc/I2C.h
        Utils/Inc/Interrupt.h
        Utils/Src/Interrupt.c
        Application/Src/Interrupt_Service.c
        Utils/Src/Delay.c
        Libs/MPU6050/MPU6050_Reg.h
        Libs/MPU6050/MPU6050.h
        Libs/MPU6050/MPU6050.c
        Libs/BH1750/BH1750.c
        Libs/BH1750/BH1750.h
        Libs/AHT20/AHT20.c
        Libs/AHT20/AHT20.h
        Libs/BMP280/BMP280.c
        Libs/BMP280/BMP280.h


)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined include paths
        Utils/Inc
        Libs/GP10
        Libs/OLCD
        Libs/BH1750
        Libs/AHT20
        Libs/BMP280
        Libs/MPU6050
        Application
        Application/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
        stm32cubemx

        # Add user defined libraries
)
