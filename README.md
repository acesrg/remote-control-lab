![remote-control-lab svg](https://user-images.githubusercontent.com/26353057/82385781-43bc0480-9a09-11ea-87a9-e8fedc28413b.png)

The remote control lab is (going to be) a control-systems *remote* laboratory. It's main goal is to aid students in the understanding of multiple control techniques, and allow them experimentally to manipulate and control multiple dynamic systems.

## Status: :construction: :construction: on development :construction: :construction:
[![pipelines](https://gitlab.com/marcomiretti/remote-control-lab/badges/master/pipeline.svg)](https://gitlab.com/marcomiretti/remote-control-lab/pipelines/latest)

## About this repo:
This repo contains the firmware for the dynamic systems, providing:
- The communication with the sensors and actuators (read and write, respectively).
- An `http` server, to set and get the formerly mentioned data.

## Build :nut_and_bolt:, burn :fire:, replicate :factory: and further herbs :herb::
### esp8266
#### Docker approach :whale::
Easiest of them all, its requirements are:
- docker

1. clone this repo
```bash
git clone https://github.com/MarcoMiretti/remote-control-lab.git
```
2. pull the containerized development-environment (one-timer)
```bash
docker pull marcotti/esp-open-rtos
```
 3. `cd` into your desired `target-system` and run the container
```bash
cd <target-system>/esp
```
in the following command, make sure not to copy and paste (i know i would), but replace `your-ssid` and `your-pass` with the target wifi ssid and password. If you intend to test this with your computer, then is the same network to which your computer is connected.

without flash support:
```bash
docker run --rm -it -e "ESPBAUD=115200" -e "WIFI_SSID=<your-ssid>" -e "WIFI_PASS=<your-pass>" -v $(pwd):/home/esp/esp-open-rtos/examples/project marcotti/esp-open-rtos /bin/bash
```
with flash support:
```bash
docker run --rm -it --privileged -e "ESPBAUD=115200" -e "WIFI_SSID=<your-ssid>" -e "WIFI_PASS=<your-pass>" -v /dev/bus/usb:/dev/bus/usb -v $(pwd):/home/esp/esp-open-rtos/examples/project marcotti/esp-open-rtos /bin/bash
```
4. run the environment-variable-replacing hackity-hack, this will replace the former inserted wifi data into the ssid config file
```bash
envsubst < include/private_ssid_config.h | sponge include/private_ssid_config.h
```
5. start developing!
to build just:
```bash
make
```
to flash
```
make flash
```

#### The i-want-all-the-toolchains approach :tired_face::

1. Follow [esp-open-rtos](https://github.com/SuperHouse/esp-open-rtos) instructions to clone and compile.

2. Manually replace the wifi parameters in `include/private_ssid_config.h`

3. `cd` into the project repo and try (5) from "the docker approach".
