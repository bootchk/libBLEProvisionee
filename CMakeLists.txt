
# Derived from Polidea's github repository cmake-nRF5x


cmake_minimum_required(VERSION 3.5.1)

project(libBLEProvisionee C CXX ASM)

set(NRF_TARGET "nrf52")

# environment (location of NRF_SDK and other tools)
include("CMakeEnv.cmake")

# scripts specific to nRF5x building
# Defines cmake macros prefixed with "nRF5x"
# Sets many cmake vars for tool flags
include("CMake_nRF5x.cmake")

nRF5x_setup()



# include API header of library
# include_directories(".")

# target depends on other libraries and their headers
# TODO other location
#include_directories("/home/bootch/git/multiProtocolNordic/src/objects")

#file(GLOB Multiprotocol_Sources ABSOLUTE "/home/bootch/git/multiProtocolNordic/src/objects" "*.cpp")

# Timer
#include_directories("/home/bootch/git/radioSoC/src/clock")

# SOURCE_FILES is mostly NRF files
# append main for our executable
list(APPEND SOURCE_FILES 
   "softdevice.cpp"
   "main2.cpp"
   "foo.cpp"
   )

# not having desired effect? 
message( "SOURCE_FILES ${SOURCE_FILES}")



# Rather than explicitly create many cmake vars and lists,
# use the newish target_<foo> functions of cmake

# "" since sources specified later by target_sources
add_executable(libBLEProvisionee "" )

# add_library(libBLEProvisionee "" )

# project uses C++ facades to SDK
target_sources(libBLEProvisionee
   PRIVATE
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdevice.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdeviceHandler.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/softdeviceSleeper.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/gap.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/nrfLog.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/appHandler.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/provisioner.cpp"
       "${CMAKE_CURRENT_LIST_DIR}/objects/timerAdaptor.cpp"
       "${SDK_SOURCE_FILES}"
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}/main2.cpp"
   )


#  Using radioSoC library

#find_library(radioSoC_LIB radioSoC)
#target_link_libraries(libBLEProvisionee PUBLIC ${radioSoc_LIB})

# ??? The library is defined as header only (INTERFACE) ???
#include(/home/bootch/git/radioSoC/src/clock/CMakeLists.txt)
#target_link_libraries(libBLEProvisionee clock)

# target_link_libraries(libBLEProvisionee "/home/bootch/git/radioSoC/Debug52/libradioSoC52.a")

# imported library target
# link directories are discourage
#LINK_DIRECTORIES("${CMAKE_BINARY_DIR}/home/bootch/git/radioSoC/Debug52")
#find_library( radioSoC52 STATIC IMPORTED GLOBAL REQUIRED)

#add_library( radioSoC52 STATIC IMPORTED)
# You can define two import-locations: one for debug and one for release.
#set_target_properties( radioSoC52 PROPERTIES IMPORTED_LOCATION "/home/bootch/git/radioSoC/Debug52/libradioSoC52.a" )

#target_link_libraries(libBLEProvisionee radioSoC52)

# target for all libraries
#add_library(LibsModule "main2.cpp")
#target_link_libraries(LibsModule libradioSoC52.a)
#target_link_libraries(LibsModule -L/home/bootch/git/radioSoC/Debug52/)

# target depends on LibsModule libraries
#target_link_libraries(libBLEProvisionee LibsModule)

target_link_libraries(libBLEProvisionee "-L/home/bootch/git/radioSoC/Debug52/"  "libradioSoC52.a" )

target_include_directories(libBLEProvisionee
   PUBLIC
       "${CMAKE_CURRENT_LIST_DIR}"
       "${CMAKE_CURRENT_LIST_DIR}/objects"
       "/home/bootch/git/radioSoC/src/clock"
       )


# target is library
# no sources in current directory
#add_library("libBLEProvisionee"   "")
#include(foo/CMakeLists.txt)


# target is executable app that calls library
# macro will also add SDK_SOURCE_FILES
#nRF5x_addExecutable(
#   "libBLEProvisionee"
#   ${SOURCE_FILES}
#   )
   
#   ${Multiprotocol_Sources}
