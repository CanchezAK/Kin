# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk/tools/pioasm"
  "C:/Users/EliteBook/Documents/rp2040/test/build/pioasm"
  "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm"
  "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm/tmp"
  "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm/src/PioasmBuild-stamp"
  "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm/src"
  "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/EliteBook/Documents/rp2040/test/build/pico-sdk/src/rp2_common/tinyusb/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
