# motordriver2018

A motor driver's source code for [Robot Triathlon](http://www.robot-triathlon.org/top.html).

## Requrements

Currently, no board specification are provided.

- Hardware
  - Micro Controller: [Microchip ATmega328PB](https://www.microchip.com/wwwproducts/en/ATmega328PB)
  - Programmer: [Atmel-ICE](https://www.microchip.com/DevelopmentTools/ProductDetails/ATATMEL-ICE) 
- Software
  - Development Environment: [Docker Desktop for Mac / Windows](https://www.docker.com/products/docker-desktop)
  - Programmer: [Arduino IDE](https://www.arduino.cc/en/main/software)

## Setup

```bash
$ docker-compose up -d
```

## Build Source Code

```bash
$ docker-compose exec main bash
$ make
```

## Write HEX File

After checking port name from Arduino IDE.

```bash
$ .\write_hex.bat COM0                     # for Windows
$ ./write_hex.sh /dev/cu.usbmodem146101    # for Mac
```

## Edit Souce Code

```bash
$ docker-compose exec main bash
$ nvim src/main.cpp
```

## Remove Docker Containers, Volumes, And Images

```bash
$ docker-compose down --rmi all --volume
```
