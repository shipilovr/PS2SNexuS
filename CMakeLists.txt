# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Paths to the official WIZnet-PICO-C repository
set(WIZNET_PORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/WIZnet-PICO-C/port/ioLibrary_Driver)
set(WIZNET_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/WIZnet-PICO-C/libraries/ioLibrary_Driver)

cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(my_udp_sender C CXX ASM)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(ps2snexus
    main.c
    ${WIZNET_PORT_DIR}/src/wizchip_spi.c
    ${WIZNET_LIBS_DIR}/Ethernet/wizchip_conf.c
    ${WIZNET_LIBS_DIR}/Ethernet/W5500/w5500.c
    ${WIZNET_LIBS_DIR}/Ethernet/socket.c
)

target_compile_definitions(ps2snexus PRIVATE
    _WIZCHIP_=W5500
    USE_SPI_DMA
)

target_include_directories(ps2snexus PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/WIZnet-PICO-C
    ${CMAKE_CURRENT_SOURCE_DIR}/WIZnet-PICO-C/port
    ${CMAKE_CURRENT_SOURCE_DIR}/WIZnet-PICO-C/port/board          # Тут лежит board_list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/WIZnet-PICO-C/port/common         # Тут лежит port_common.h
    ${WIZNET_PORT_DIR}/inc
    ${WIZNET_LIBS_DIR}/Ethernet
    ${WIZNET_LIBS_DIR}/Ethernet/W5500
)

target_link_libraries(ps2snexus 
    pico_stdlib
    hardware_spi
    hardware_dma
    hardware_timer
)

pico_enable_stdio_usb(ps2snexus 1)
pico_enable_stdio_uart(ps2snexus 0)

pico_add_extra_outputs(ps2snexus)
