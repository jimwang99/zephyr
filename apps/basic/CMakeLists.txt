# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(circular-buffer)

target_sources(app PRIVATE src/main.cpp src/circular_buffer.cpp)
