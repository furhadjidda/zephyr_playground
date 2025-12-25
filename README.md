# Zephyr Playground

## Goal
Goal of this repo is to play with zephyr and learn about it and eventually make it work with microros for boards I have for my robotics projects

## Build instructions
You will need to follow these instructions to be able to create an environment for west
https://docs.zephyrproject.org/latest/develop/getting_started/index.html
once done you need to activate the virtual environment to be able to build the repo and apps

1. Clone the repo
2. Initialize the repo `west init .`
3. Update the repo `west update`
4. Go to the app you want to build and build.
    * For example if you are using the pimoroni pico plus 2 W board this is the command `west build -p always -b pico_plus2/rp2350b/m33 -- -DEXTRA_CONF_FILE=boards/rpi_pico.conf -DDTC_OVERLAY_FILE=boards/rpi_pico.overlay`
    * For ESP32 S3 Devkit C use this
    `west build -p always -b esp32s3_devkitc/esp32s3/procpu --sysbuild . -- -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay`
    * For Adafruit Feather ESP32 S3
    `west build -p always -b adafruit_feather_esp32s3/esp32s3/procpu --sysbuild . -- -DDTC_OVERLAY_FILE=boards/adafruit_feather_s3.overlay`
5. To Flash you can use `west flash --esp-device <device_path>`


Note # 1: All the Zephyr supported board are listed here - https://docs.zephyrproject.org/latest/boards/index.html#

Note # 2: At the time of writing this i am using these versions
```
CMake version: 3.22.1
Python version 3.10.12
Zephyr version: 4.3.99
Zephyr SDK version 0.17.4
West version 1.5
```

## Building with a different Zephyr Version(4.1)
```
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v4.1-branch
west update
```

You will also need to update the SDK to be compatible
```
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5/zephyr-sdk-0.16.5_linux-x86_64.tar.xz
tar xf zephyr-sdk-0.16.5_linux-x86_64.tar.xz
cd zephyr-sdk-0.16.5
./setup.sh
```
This build with option `-DZEPHYR_SDK_INSTALL_DIR=$HOME/zephyr-sdk-0.16.5`

### Example
`west build -p always -b esp32s3_devkitc/esp32s3/procpu --sysbuild . -- -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay -DZEPHYR_SDK_INSTALL_DIR=/projects/github_repos/zephyr_playground/zephyr_with_microros/zephyr-sdk-0.16.5`

### Some command for references
```
west build -p always -b arduino_nicla_vision/stm32h747xx/m7 --sysbuild . -- -DDTC_OVERLAY_FILE=boards/nicla-vision.overlay -DEXTRA_CONF_FILE=boards/nicla.conf

west flash

west build -p always -b adafruit_feather_esp32s3/esp32s3/procpu --sysbuild . -- -DDTC_OVERLAY_FILE=boards/adafruit_feather_s3.overlay -DEXTRA_CONF_FILE=boards/adafruit_feather_s3.conf

west flash --esp-device /dev/adafruit_esp32s3_zephyr

west build -p always -b esp32s3_devkitc/esp32s3/procpu  . -- -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay -DEXTRA_CONF_FILE=boards/esp32_s3.conf

west flash --esp-device /dev/espressif_esp32_s3

```