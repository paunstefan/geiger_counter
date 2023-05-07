# Raspberry Pi Pico Geiger Counter

Geiger Counter based on the RadiationD v1.1 (CAJOE) board, with a J305 geiger tube.

## Build

To build the Raspberry Pi Pico binary:

```
mkdir build
cd build
cmake ..
make
```

In the `build/src/` directory there will be a `geiger_counter.uf2` file that can be uploaded to the RPi. 
Hold the BOOTSEL button on the board and insert the USB cable, the Pico should be mounted as a removable drive,
Copy the `.uf2` file to it.