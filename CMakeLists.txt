
cmake_minimum_required(VERSION 3.5.1)

project(BLEProvisionee C CXX ASM)


# cmake includes modules from here
# !!! only for simple names without the .cmake suffix
set(CMAKE_MODULE_PATH "/home/bootch/git/nRF5Cmake/")
message("${CMAKE_MODULE_PATH}")


# environment (location of NRF_SDK and other tools)
# i.e. file CMakeEnv.cmake on the MODULE_PATH
include("CMakeEnv")

# set var that configures CMake_nRF5x
set(NRF_FAMILY "nrf52")

set(CHIP "nrf52832")
set(SOFTDEVICE "s132")
# chip has FPU but we elect not to use it
set(FLOAT_ABI "soft")
# also below:  Debug52

#set(CHIP "nrf52810")
#set(SOFTDEVICE "s112")
# also below: Release52


# scripts specific to nRF5x building
# Defines cmake macros prefixed with "nRF5"
# Sets many cmake vars for tool flags
include("nRF5")

nRF5CheckSetupPreconditions()

# Must set linker script before build options
# Broken for my use:  nRF5SetSoftdeviceLinkerScript(NRF5_LINKER_SCRIPT "s132")
# Only for the text harness target
set(NRF5_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/gcc_nrf52.ld")

nRF5SetBuildOptions()
nRF5SetIncludePaths()


nRF5SetSDKSourcesBasic()
# Also BLE
nRF5SDKSourcesBLE()
# SDK_SOURCES is now a list of SDK source files called by library and archived in library

# define sources needed startup an executable (the test harness)
nRF5SDKSourcesStartup()

# Needed if logging enabled in sdk_config.h
#nRF5x_addNRF_LOG()
#nRF5x_addRTT()


# build lib

# note the logical name does not have prefix "lib", which cmake adds 
# note sources specified later
add_library(BLEProvisionee "" )

nRF5SetSoftdeviceIncludePaths( "BLEProvisionee" ${SOFTDEVICE})
nRF5SetSoftdeviceDefinitions( "BLEProvisionee" ${SOFTDEVICE})
nRF5SetChipCompileDefinitions( "BLEProvisionee" ${CHIP})

# used only by custom target FLASH_SOFTDEVICE
# TODO use it for more customer targets, one per SD 
nRF5SetSoftdevicePaths(SOFTDEVICE_PATH ${SOFTDEVICE})

# import libs from outside project

# libBLEProvisionee includes headers from library radioSoC52
# WAS Debug52wSD
add_library(radioSoC52wSD STATIC IMPORTED)
set_target_properties( radioSoC52wSD
    PROPERTIES 
        IMPORTED_LOCATION /home/bootch/git/radioSoC/Debug52wSD/libradioSoC52wSD.a
        INTERFACE_INCLUDE_DIRECTORIES "/home/bootch/git/radioSoC/src/"
    )

add_library(nRF5x STATIC IMPORTED)
set_target_properties( nRF5x PROPERTIES IMPORTED_LOCATION /home/bootch/git/nRF5x/Debug52/libnRF5x52.a )

add_library(embeddedMath STATIC IMPORTED)
set_target_properties( embeddedMath PROPERTIES IMPORTED_LOCATION /home/bootch/git/embeddedMath/Debug52/libembeddedMath52.a )

add_library(NRFDrivers STATIC IMPORTED)
set_target_properties( NRFDrivers PROPERTIES IMPORTED_LOCATION /home/bootch/git/libNRFDrivers/Debug/libNRFDrivers.a )



IF(EXISTS /home/bootch/git/radioSoC/Debug52wSD/libradioSoC52wSD.a)
else()
    message("FATAL_ERROR libradioSoC52.a not exist")
ENDIF()

IF(EXISTS /home/bootch/git/nRF5x/Debug52/libnRF5x52.a )
else()
    message("FATAL_ERROR libnRF5x52.a not exist")
ENDIF()

IF(EXISTS /home/bootch/git/embeddedMath/Debug52/libembeddedMath52.a)
else()
    message("FATAL_ERROR libembeddedMath52.a not exist")
ENDIF()


# test harness that calls the lib
# source declared later
add_executable(testLibBLEProvisionee "")

# executable target for same chip and softdevice
nRF5SetSoftdeviceIncludePaths( "testLibBLEProvisionee" ${SOFTDEVICE})
nRF5SetSoftdeviceDefinitions( "testLibBLEProvisionee" ${SOFTDEVICE})
nRF5SetChipCompileDefinitions( "testLibBLEProvisionee" ${CHIP})



# harness links target library (using a logical name, from preceding add_library()
# the library built by this project
# and external libraries
target_link_libraries(testLibBLEProvisionee 
PRIVATE
    BLEProvisionee
    radioSoC52wSD
    nRF5x
    embeddedMath
    NRFDrivers
    )


# lib uses C++ facades to SDK
# AND radioSoC lib
# TODO why doesn't the above declaration for radioSoC make the headers available?
target_include_directories(BLEProvisionee
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}"
       "${CMAKE_CURRENT_LIST_DIR}/objects"
       "/home/bootch/git/radioSoC/src"
       )
 
 
 list(APPEND FacadeSources
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdevice.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdeviceHandler.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdeviceSleeper.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/gap.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/nrfLog.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/appHandler.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/provisioner.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/timerAdaptor.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/protocolStack.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/bleProtocol.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/gatt.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/advertiser.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/advertiserDirect.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/advertisement.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/service.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/characteristic.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/uuid.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/rssi.cpp"
       )
 


# sources for lib are just the facade classes
# !!! And the SDK_SOURCE_FILES they call (so that the app (exe) does not need to link them)
target_sources(BLEProvisionee
   PUBLIC
       "${FacadeSources}"
       "${SDK_SOURCES}"
   )
   
   
# sources of test harness
# a main which calls the library
# and SDK sources that startup the app
# choose main.cpp or main2.cpp (minimal)
target_sources(testLibBLEProvisionee
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
       "${SDK_SOURCES_STARTUP}"
   )
 
 
nRF5GenerateOtherArtifacts(testLibBLEProvisionee)
nRF5AddCustomTargets(testLibBLEProvisionee)

message("SDK_SOURCES:  ${SDK_SOURCES}")
message("SDK_SOURCES_STARTUP: ${SDK_SOURCES_STARTUP}")



