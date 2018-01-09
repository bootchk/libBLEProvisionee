
# Derived from Polidea's github repository cmake-nRF5x


cmake_minimum_required(VERSION 3.5.2)

project(libBLEProvisionee C CXX ASM)

set(NRF_TARGET "nrf52")

include("CMakeEnv.cmake")
include("CMake_nRF5x.cmake")

nRF5x_setup()

# include API header of library
include_directories(".")

# TODO other location
include_directories("/home/bootch/git/multiProtocolNordic/src/objects")
# Timer
include_directories("/home/bootch/git/radioSoC/src/clock")

list(APPEND SOURCE_FILES "main.cpp")

nRF5x_addExecutable(${PROJECT_NAME} "${SOURCE_FILES}")
