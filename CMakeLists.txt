# Cmake File for building and flashing the Atmega328p.
# This makes it possible to build the project on every platform that supports cmake, avdude and has a c compile toolchain installed.
# The directories for these tool can be set in line 45-48.
# To build the project run "cmake ." from the root directory.
# To flash the Atmega328p connect it to one of your USB ports and run "make" from the root repository.

CMAKE_MINIMUM_REQUIRED(VERSION 3.16.0)
SET(CMAKE_SYSTEM_NAME Generic)

#============================================================================================
PROJECT(ME_BIKE)      # <-- Projektname

SET(FILES
        main.c
        main.h
        uart/serial.c
        uart/serial.h
        uart/softuart.c
        uart/softuart.h
        gps/gps_main.c
        gps/gps_main.h
        adc/adc.c
        adc/adc.h
        bluetooth/bluetooth.c
        bluetooth/bluetooth.h
        hall/hall_main.c
        hall/hall_main.h
        BNO055/bno055_main.h
        BNO055/bno055_main.c
        hall/adcHall/adcHall.c
        hall/adcHall/adcHall.h
        i2cmaster/twimaster.c
        i2cmaster/i2cmaster.h
        pulsesensor/pulsesensor.h
        pulsesensor/pulsesensor.c
        util/Interrupt/timer.c
        util/Interrupt/timer.h
        )
# <-- Quelldatei(en)

SET(DEVICE atmega328p)   # <-- MCU
SET(FREQ 16000000)   # <-- MCU-Frequenz
SET(F_CPU 16000000)

SET(AVRBIN /usr/local/bin)  # <-- Toolchain
SET(AVROBJCOPY avr-objcopy)  # <--     ""
SET(AVRSIZE avr-size)   # <--     ""
SET(AVRDUDE avrdude)   # <--     ""

SET(PROGRAMMER USBasp)  # <-- Programmertype
SET(PORT usb)            # <-- USB bzw. Schnittstelle z.B. /dev/cu.usbserial....
# SET(BAUD 57600)	        	# <-- Übertragungsrate

SET(EEPROM NO)                # <-- soll eine x.eep Datei erstellt werden?
SET(FLASH YES)           # <-- soll Geflasht werden?

#============================================================================================

SET(CMAKE_C_COMPILER ${AVRBIN}/avr-gcc)
SET(CMAKE_CXX_COMPILER ${AVRBIN}/avr-g++)

SET(CMAKE_C_FLAGS "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -std=gnu99 -Wl,--gc-sections")
SET(CMAKE_CXX_FLAGS "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -Wl,--gc-sections")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_PREFIX_PATH /opt/local/avr)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.eep"
        )

include_directories(/opt/local/avr/include)

SET(SOURCE_FILES ${FILES})

#============================================================================================

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${FILES} main.c main.h)

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVROBJCOPY} -O ihex -R .eeprom ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex)

if (EEPROM)
    ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVROBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.eep)
endif (EEPROM)

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVRSIZE} --target=ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex)

if (FLASH)
    ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVRDUDE} -p${DEVICE} -c${PROGRAMMER} -P${PORT} -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex)
endif (FLASH)

set(CMAKE_C_STANDARD 99)
