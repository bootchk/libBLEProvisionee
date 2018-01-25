
#

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
set(NRF_TARGET "nrf52")
set(NRF_SOFTDEVICE "s132")


# scripts specific to nRF5x building
# Defines cmake macros prefixed with "nRF5x"
# Sets many cmake vars for tool flags
include("nRF5")

nRF5CheckSetupPreconditions()
nRF5SetBuildOptions()
nRF5SetIncludePaths()
nRF5SetSDKSourcesBasic()

# Also BLE
nRF5SDKSourcesBLE()



# build lib

# note the logical name does not have prefix "lib", which cmake adds 
# note sources specified later
add_library(BLEProvisionee "" )



# import libs from outside project

# libBLEProvisionee includes headers from library radioSoC52
add_library(radioSoC52 STATIC IMPORTED)
set_target_properties( radioSoC52 
    PROPERTIES 
        IMPORTED_LOCATION /home/bootch/git/radioSoC/Debug52/libradioSoC52.a
        INTERFACE_INCLUDE_DIRECTORIES "/home/bootch/git/radioSoC/src/clock"
    )

add_library(nRF5x STATIC IMPORTED)
set_target_properties( nRF5x PROPERTIES IMPORTED_LOCATION /home/bootch/git/nRF5x/DebugnRF52/libnRF5x52.a )

add_library(embeddedMath STATIC IMPORTED)
set_target_properties( embeddedMath PROPERTIES IMPORTED_LOCATION /home/bootch/git/embeddedMath/Debug52/libembeddedMath52.a )

add_library(NRFDrivers STATIC IMPORTED)
set_target_properties( NRFDrivers PROPERTIES IMPORTED_LOCATION /home/bootch/git/libNRFDrivers/Debug/libNRFDrivers.a )



IF(EXISTS /home/bootch/git/radioSoC/Debug52/libradioSoC52.a)
else()
    message("FATAL_ERROR libradioSoC52.a not exist")
ENDIF()

IF(EXISTS /home/bootch/git/nRF5x/DebugnRF52/libnRF5x52.a )
else()
    message("FATAL_ERROR libnRF5x52.a not exist")
ENDIF()

IF(EXISTS /home/bootch/git/embeddedMath/Debug52/libembeddedMath52.a)
else()
    message("FATAL_ERROR libembeddedMath52.a not exist")
ENDIF()


#if(TARGET radioSoC52)



# test harness that calls the lib
# source declared later
add_executable(testLibBLEProvisionee "")


# harness links target library (using a logical name, from preceding add_library()
# the library built by this project
# and external libraries
target_link_libraries(testLibBLEProvisionee 
PRIVATE
    BLEProvisionee
    radioSoC52
    nRF5x
    embeddedMath
    NRFDrivers
    )

# TODO radioSoC52SD


# lib uses C++ facades to SDK
# AND radioSoC lib
# TODO why doesn't the above declaration for radioSoC make the headers available?
target_include_directories(BLEProvisionee
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}"
       "${CMAKE_CURRENT_LIST_DIR}/objects"
       "/home/bootch/git/radioSoC/src/clock"
       )
     
       
# message("${SDK_SOURCES}")

# sources for lib are just the facade classes
# Not the SDK_SOURCE_FILES they call.
target_sources(BLEProvisionee
   PUBLIC
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
   )
   
   
# sources of test harness
# a main which calls the library
# and SDK sources to satisfy calls from the library
# choose main.cpp or main2.cpp (minimal)
target_sources(testLibBLEProvisionee
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
       "${SDK_SOURCES}"
   )
 
 
#nRF5GenerateOtherArtifacts(testLibBLEProvisionee)
#nRF5AddCustomTargets(testLibBLEProvisionee)
