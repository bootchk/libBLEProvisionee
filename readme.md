libBLEProvisionee

work in progress

Attributes
-
   - library
   - for Nordic nRF5x family SoC
   - uses Bluetooth BLE
   - provides provisioning service: writeable config vars
   - NRF SDK 14.2

Brief
-

A provisioner app runs on a smart phone.  See https://github.com/bootchk/BLEProvisionerSwift.  Provisioner talks to provisionee using BT protocol.

The library runs a provisioning session.  
During a session, the library advertises its service: able to be provisioned.
A provisioner app scans for a provisionee; when it finds one it uses the service (writes a BT characteristic.)
The provisionee (library) disconnects after every write and ends the session.
Sessions timeout, are short duration, and use little power.
A calling app (to the library) can determine any provisioned value.

After a provisioning session, the calling app is free to use the radio and other HW resources,
in a constrained fashion as documented for the Nordic Softdevice.
That is, the library uses the Softdevice compatibly with multiprotocol.
Multiprotocol means the app can use the Softdevice for any other protocol or BT again.

A provisionable app must run provisioning sessions often enough,
and a provisioner app must scan long enough,
for success.

Library
-

An app linking to the library must provide the startup code for an app.

Dependencies
-

TODO


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
