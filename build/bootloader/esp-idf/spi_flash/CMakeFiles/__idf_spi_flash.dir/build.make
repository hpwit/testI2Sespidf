# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yvesbazin/esp/esp-idf/components/bootloader/subproject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yvesbazin/esp/projects/hello_world/build/bootloader

# Include any dependencies generated for this target.
include esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/depend.make

# Include the progress variables for this target.
include esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/progress.make

# Include the compile flags for this target's objects.
include esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/flags.make

esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.obj: esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/flags.make
esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.obj: /Users/yvesbazin/esp/esp-idf/components/spi_flash/esp32/spi_flash_rom_patch.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yvesbazin/esp/projects/hello_world/build/bootloader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.obj"
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash && /Users/yvesbazin/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.obj -c /Users/yvesbazin/esp/esp-idf/components/spi_flash/esp32/spi_flash_rom_patch.c

esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.i"
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash && /Users/yvesbazin/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/yvesbazin/esp/esp-idf/components/spi_flash/esp32/spi_flash_rom_patch.c > CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.i

esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.s"
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash && /Users/yvesbazin/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/yvesbazin/esp/esp-idf/components/spi_flash/esp32/spi_flash_rom_patch.c -o CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.s

# Object files for target __idf_spi_flash
__idf_spi_flash_OBJECTS = \
"CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.obj"

# External object files for target __idf_spi_flash
__idf_spi_flash_EXTERNAL_OBJECTS =

esp-idf/spi_flash/libspi_flash.a: esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/esp32/spi_flash_rom_patch.c.obj
esp-idf/spi_flash/libspi_flash.a: esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/build.make
esp-idf/spi_flash/libspi_flash.a: esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yvesbazin/esp/projects/hello_world/build/bootloader/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libspi_flash.a"
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash && $(CMAKE_COMMAND) -P CMakeFiles/__idf_spi_flash.dir/cmake_clean_target.cmake
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/__idf_spi_flash.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/build: esp-idf/spi_flash/libspi_flash.a

.PHONY : esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/build

esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/clean:
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash && $(CMAKE_COMMAND) -P CMakeFiles/__idf_spi_flash.dir/cmake_clean.cmake
.PHONY : esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/clean

esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/depend:
	cd /Users/yvesbazin/esp/projects/hello_world/build/bootloader && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yvesbazin/esp/esp-idf/components/bootloader/subproject /Users/yvesbazin/esp/esp-idf/components/spi_flash /Users/yvesbazin/esp/projects/hello_world/build/bootloader /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash /Users/yvesbazin/esp/projects/hello_world/build/bootloader/esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/depend

