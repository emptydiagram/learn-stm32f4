#!/bin/sh

F4_ROOT_DIR=$1

if [ -z $F4_ROOT_DIR ]; then
  printf 'Error: missing path to extracted folder from STM32CubeF4 zip.\nUsage: %s <STM32CubeF4 root dir>\n', "$0" >&2
  exit 1
fi

mkdir -p chip_headers
cp -r $F4_ROOT_DIR/Drivers/CMSIS/Device/ST/STM32F4xx/Include/ chip_headers/STM32F4-Include
cp -r $F4_ROOT_DIR/Drivers/CMSIS/Include/ chip_headers/Core-Include
