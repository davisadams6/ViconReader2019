# Vicon Reader (2019)

A simple Vicon Reader using a mix of C/C++ for Linux systems on the LASR network.
Tested on Ubuntu 14.04.

## Dependencies
* CMake 2.8 or newer
* ZeroMQ message library (install packages `libzmq3` and `libzmq3-dev`. Tested with version 4.0.4
* Linux

## Compiling from scratch and running
* From a terminal, navigate to the `build` folder (should be empty after cloning)
* Run `cmake ../` to create the needed build files
* Run `make` to build the reader and examples
* Run `./reader` from the `build` directory
	* If ViconBroadcaster2019.exe is running on the Vicon PC, you'll see parsed output indicating the frame number and subject name and data
	* If the broadcaster is not running or is not working correctly, you should see no output
* <CTRL+C> to exit