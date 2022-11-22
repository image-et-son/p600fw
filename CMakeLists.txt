cmake_minimum_required(VERSION 3.11)

# AVR GCC toolchain (has to be set before project())
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

project(p600fw)
set(PRODUCT_NAME p600firmware)

# AVR chip configuration
set(F_CPU 16000000UL)
set(MCU at90usb1286)

# Pass defines to compiler
add_definitions(
  -DF_CPU=${F_CPU}
)

add_compile_options(
  -mmcu=${MCU} # MCU
  -std=gnu99 # C99 standard
  -O2 # optimize
  -Wall # enable warnings
  -gdwarf-2
  -funsigned-char
  -funsigned-bitfields
  -fpack-struct
  -fshort-enums
  -Wstrict-prototypes
  -fno-unroll-loops
  -fno-inline-functions
  -flto=auto
  -fuse-linker-plugin 
)

add_link_options(
  -mmcu=${MCU}
  -flto=auto
  -Wl,-Map=p600firmware.map,--cref
  -Wl,--relax
  -lm
  -Wl,--section-start=.updater=0x1e000
  -Wl,--section-start=.nrww_misc=0x1ec00
  -Wl,--section-start=.bootloader=0x1f000
)

include_directories(
  common
  firmware
)

file(GLOB SRC_FILES
  "xnormidi/*.c"
  "xnormidi/bytequeue/*.c"
  "firmware/*.c"
  "common/*.c"
)

# Generate list files
set(CMAKE_C_COMPILE_OBJECT "${CMAKE_C_COMPILE_OBJECT} -Wa,-adhlns=<OBJECT>.lst")

# Create one target for compiling
add_executable(${PRODUCT_NAME} EXCLUDE_FROM_ALL ${SRC_FILES})
set_target_properties(${PRODUCT_NAME} PROPERTIES SUFFIX .elf ADDITIONAL_CLEAN_FILES ${PRODUCT_NAME}.map)

# Call avr-size after building .elf file
add_custom_target(size ALL avr-size ${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})

add_custom_command(
  OUTPUT ${PRODUCT_NAME}.hex
  COMMAND avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature ${PRODUCT_NAME}.elf ${PRODUCT_NAME}.hex
  DEPENDS ${PRODUCT_NAME}
)
add_custom_target(hex DEPENDS ${PRODUCT_NAME}.hex)

add_custom_command(
  OUTPUT ${PRODUCT_NAME}.lss
  COMMAND avr-objdump -h -S -z ${PRODUCT_NAME}.elf > ${PRODUCT_NAME}.lss
  DEPENDS ${PRODUCT_NAME}
)
add_custom_target(lss DEPENDS ${PRODUCT_NAME}.lss)

add_custom_command(
  OUTPUT ${PRODUCT_NAME}.bin
  COMMAND avr-objcopy -O binary -R .eeprom -R .fuse -R .lock -R .signature -R .updater -R .nrww_misc -R .bootloader ${PRODUCT_NAME}.elf ${PRODUCT_NAME}.bin
  DEPENDS ${PRODUCT_NAME}
)
add_custom_target(bin DEPENDS ${PRODUCT_NAME}.bin)

add_custom_command(
  OUTPUT ${PRODUCT_NAME}.syx
  COMMAND python3 "${CMAKE_SOURCE_DIR}/fw2syx/fw2syx.py" -o ${PRODUCT_NAME}.syx ${PRODUCT_NAME}.bin
  DEPENDS bin
)
add_custom_target(syx ALL DEPENDS ${PRODUCT_NAME}.syx)

add_custom_target(sendsyx p600_send ${PRODUCT_NAME}.syx DEPENDS ${PRODUCT_NAME}.syx)
