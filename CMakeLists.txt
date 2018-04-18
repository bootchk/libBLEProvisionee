
cmake_minimum_required(VERSION 3.5.1)

project(BLEProvisionee C CXX ASM)


# cmake includes modules from here
# !!! only for simple names without the .cmake suffix
set(CMAKE_MODULE_PATH "/home/bootch/git/nRF5Cmake/")
message("${CMAKE_MODULE_PATH}")


# environment (location of NRF_SDK and other tools)
# i.e. file CMakeEnv.cmake on the MODULE_PATH
include("CMakeEnv")

# also below:  Debug52 vs Release52
# should name the libraries for their combo, i.e. libFoo_s132_opt


# scripts specific to nRF5x building
# Defines cmake macros prefixed with "nRF5"
# Sets many cmake vars for tool flags
include("nRF5")

nRF5CheckSetupPreconditions()

nRF5SetBuildOptions()
nRF5SetIncludePaths()


nRF5SetSDKSourcesBasic()
# Also BLE
nRF5SDKSourcesBLE()
# SDK_SOURCES is now a list of SDK source files called by library and archived in library

# Needed if logging enabled in sdk_config.h
#nRF5x_addNRF_LOG()
#nRF5x_addRTT()


# build lib

# note the logical name does not have prefix "lib", which cmake adds 
# note sources specified later
add_library(BLEProvisionee "" )
set_target_properties( BLEProvisionee
    PROPERTIES
       CHIP "nrf52832_xxaa"
       SOFTDEVICE "s132"
       FLOAT_ABI "soft"
    )
nRF5ConfigTargetByProperties(BLEProvisionee)



add_library(BLEProvisionee52810e "" )
set_target_properties( BLEProvisionee52810e
    PROPERTIES
       CHIP "nrf52810e"
       SOFTDEVICE "s112"
       FLOAT_ABI "soft"
    )
nRF5ConfigTargetByProperties(BLEProvisionee52810e)



# used only by custom target FLASH_SOFTDEVICE
# TODO use it for more customer targets, one per SD 
nRF5SetSoftdevicePaths(SOFTDEVICE_PATH "s132")

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
set_target_properties( embeddedMath PROPERTIES 
    IMPORTED_LOCATION /home/bootch/git/embeddedMath/Debug52/libembeddedMath52.a 
    INTERFACE_INCLUDE_DIRECTORIES "/home/bootch/git/embeddedMath/src/")

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
set_target_properties( testLibBLEProvisionee
   PROPERTIES
       CHIP "nrf52832_xxaa"
       SOFTDEVICE "s132"
       FLOAT_ABI "soft"
   )
nRF5ConfigTargetByProperties(testLibBLEProvisionee)
nRF5SetTargetsLinkerScript(testLibBLEProvisionee "${CMAKE_SOURCE_DIR}/gcc_nrf52.ld")



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
# Same for 52810
target_include_directories(BLEProvisionee52810e
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}"
       "${CMAKE_CURRENT_LIST_DIR}/objects"
       "/home/bootch/git/radioSoC/src"
       )
 
 
 list(APPEND FacadeSources
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdevice.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdeviceHandler.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/gap.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/nrfLog.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/appHandler.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/provisioner.cpp"
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
 
 #obsolete
 #       "${CMAKE_CURRENT_LIST_DIR}/objects/timerAdaptor.cpp"
 #      "${CMAKE_CURRENT_LIST_DIR}/objects/softdeviceSleeper.cpp"


# sources for lib are just the facade classes
# !!! And the SDK_SOURCE_FILES they call (so that the app (exe) does not need to link them)
target_sources(BLEProvisionee
   PUBLIC
       "${FacadeSources}"
       "${SDK_SOURCES}"
   )
# Same sources for 52810 variant emulated
target_sources(BLEProvisionee52810e
   PUBLIC
       "${FacadeSources}"
       "${SDK_SOURCES}"
   )   
   
# sources of test harness
# a main which calls the library
# and SDK sources that startup the app
# choose main.cpp or main2.cpp (minimal)
nRF5SetTargetStartupSources(testLibBLEProvisionee)
target_sources(testLibBLEProvisionee
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
   )
 

# No need for other artifacts, not burned to ROM independently
nRF5GenerateOtherArtifacts(testLibBLEProvisionee)
#nRF5AddCustomTargets(testLibBLEProvisionee)

message("SDK_SOURCES:  ${SDK_SOURCES}")



