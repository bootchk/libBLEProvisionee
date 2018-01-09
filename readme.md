libBLEProvisionee

work in progress

Attributes
-
   - library
   - for Nordic nRF5x family SoC
   - uses Bluetooth BLE
   - provides provisioning service: writeable config vars
   - NRF SDK 14.2

Building
-

Uses CMake and a command line.

CMake scripts derived from https://github.com/Polidea/cmake-nRF5x

cmake -H. -B"cmake-build" -G "Unix Makefiles"
cmake --build "cmake-build" --target libBLEProvisionee

The former creates the build directory and Makefiles.
The latter builds.

TODO
-

get it working
another target for a test app
test multiprotocol
