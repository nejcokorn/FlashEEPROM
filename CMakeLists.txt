# v3.21 implemented semantic changes regarding $<TARGET_OBJECTS:...>
# See https://cmake.org/cmake/help/v3.21/command/target_link_libraries.html#linking-object-libraries-via-target-objects
cmake_minimum_required(VERSION 3.21)

add_library(FlashEEPROM INTERFACE)
add_library(FlashEEPROM_usage INTERFACE)

target_include_directories(FlashEEPROM_usage INTERFACE
  src
)


target_link_libraries(FlashEEPROM_usage INTERFACE
  base_config
)

target_link_libraries(FlashEEPROM INTERFACE FlashEEPROM_usage)



add_library(FlashEEPROM_bin OBJECT EXCLUDE_FROM_ALL
  src/utility/stm32_flasheeprom.c
)
target_link_libraries(FlashEEPROM_bin_bin PUBLIC FlashEEPROM_bin_usage)

target_link_libraries(FlashEEPROM_bin INTERFACE
  FlashEEPROM_bin_bin
  $<TARGET_OBJECTS:FlashEEPROM_bin_bin>
)

