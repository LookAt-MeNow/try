# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zhang/net_design

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhang/net_design/build

# Include any dependencies generated for this target.
include include/mysql_connection/CMakeFiles/mysql_connection.dir/depend.make

# Include the progress variables for this target.
include include/mysql_connection/CMakeFiles/mysql_connection.dir/progress.make

# Include the compile flags for this target's objects.
include include/mysql_connection/CMakeFiles/mysql_connection.dir/flags.make

include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o: include/mysql_connection/CMakeFiles/mysql_connection.dir/flags.make
include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o: ../include/mysql_connection/mysql_connect.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhang/net_design/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o"
	cd /home/zhang/net_design/build/include/mysql_connection && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mysql_connection.dir/mysql_connect.c.o   -c /home/zhang/net_design/include/mysql_connection/mysql_connect.c

include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mysql_connection.dir/mysql_connect.c.i"
	cd /home/zhang/net_design/build/include/mysql_connection && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zhang/net_design/include/mysql_connection/mysql_connect.c > CMakeFiles/mysql_connection.dir/mysql_connect.c.i

include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mysql_connection.dir/mysql_connect.c.s"
	cd /home/zhang/net_design/build/include/mysql_connection && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zhang/net_design/include/mysql_connection/mysql_connect.c -o CMakeFiles/mysql_connection.dir/mysql_connect.c.s

include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.requires:

.PHONY : include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.requires

include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.provides: include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.requires
	$(MAKE) -f include/mysql_connection/CMakeFiles/mysql_connection.dir/build.make include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.provides.build
.PHONY : include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.provides

include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.provides.build: include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o


# Object files for target mysql_connection
mysql_connection_OBJECTS = \
"CMakeFiles/mysql_connection.dir/mysql_connect.c.o"

# External object files for target mysql_connection
mysql_connection_EXTERNAL_OBJECTS =

lib/libmysql_connection.a: include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o
lib/libmysql_connection.a: include/mysql_connection/CMakeFiles/mysql_connection.dir/build.make
lib/libmysql_connection.a: include/mysql_connection/CMakeFiles/mysql_connection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhang/net_design/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library ../../lib/libmysql_connection.a"
	cd /home/zhang/net_design/build/include/mysql_connection && $(CMAKE_COMMAND) -P CMakeFiles/mysql_connection.dir/cmake_clean_target.cmake
	cd /home/zhang/net_design/build/include/mysql_connection && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mysql_connection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
include/mysql_connection/CMakeFiles/mysql_connection.dir/build: lib/libmysql_connection.a

.PHONY : include/mysql_connection/CMakeFiles/mysql_connection.dir/build

include/mysql_connection/CMakeFiles/mysql_connection.dir/requires: include/mysql_connection/CMakeFiles/mysql_connection.dir/mysql_connect.c.o.requires

.PHONY : include/mysql_connection/CMakeFiles/mysql_connection.dir/requires

include/mysql_connection/CMakeFiles/mysql_connection.dir/clean:
	cd /home/zhang/net_design/build/include/mysql_connection && $(CMAKE_COMMAND) -P CMakeFiles/mysql_connection.dir/cmake_clean.cmake
.PHONY : include/mysql_connection/CMakeFiles/mysql_connection.dir/clean

include/mysql_connection/CMakeFiles/mysql_connection.dir/depend:
	cd /home/zhang/net_design/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhang/net_design /home/zhang/net_design/include/mysql_connection /home/zhang/net_design/build /home/zhang/net_design/build/include/mysql_connection /home/zhang/net_design/build/include/mysql_connection/CMakeFiles/mysql_connection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : include/mysql_connection/CMakeFiles/mysql_connection.dir/depend

