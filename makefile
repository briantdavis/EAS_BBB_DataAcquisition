#
# EAS Project
#
# Embry-Riddle Aeronautical University - Prescott Campus
# B.Davis
#
# Here is a simple Make Macro.
LINK_TARGET = beta_test_one.v1.exe

# Here is a Make Macro that uses the backslash to extend to multiple lines.
OBJS =  \
	eas_i2c_daq.o \
  eas_daq_pack.o \
  bmp180.o \
  bme280.o \
  ADXL345Accelerometer.o \
  pca9544mux.o \
  hscPress.o \
  mpu6050AccelGyro.o

# Here is a Make Macro defined by two Macro Expansions.
# A Macro Expansion may be treated as a textual replacement of the Make Macro.
# Macro Expansions are introduced with $ and enclosed in (parentheses).
REBUILDABLES = $(OBJS) $(LINK_TARGET)

# The rule for "all" is used to incrementally build your system.
# It does this by expressing a dependency on the results of that system,
# which in turn have their own rules and dependencies.
all : $(LINK_TARGET)
	echo All done

# Here is a simple Rule (used for "cleaning" your build environment).
# It has a Target named "clean" (left of the colon ":" on the first line),
# no Dependencies (right of the colon),
# and two Commands (indented by tabs on the lines that follow).
# The space before the colon is not required but added here for clarity.
clean : 
	rm -f $(REBUILDABLES)
	echo Clean done

# There are two standard Targets your Makefile should probably have:
# "all" and "clean", because they are often command-line Goals.
# Also, these are both typically Artificial Targets, because they don't typically
# correspond to real files named "all" or "clean".  

# There is no required order to the list of rules as they appear in the Makefile.
# Make will build its own dependency tree and only execute each rule only once
# its dependencies' rules have been executed successfully.

# Here is a Rule that uses some built-in Make Macros in its command:
# $@ expands to the rule's target, in this case "test_me.exe".
# $^ expands to the rule's dependencies, in this case the three files
# main.o, test1.o, and  test2.o.
$(LINK_TARGET) : $(OBJS) ./libs/tc/tinycon.o
	g++ -g -Wall -o $@ $^


 #
 # Specific Rules
./libs/tc/tinycon.o : ./libs/tc/tinycon.cpp
  g++ -g -std=c++0x -o $@ -c $<
  
# Here is a Pattern Rule, often used for compile-line.
# It says how to create a file with a .o suffix, given a file with a .cpp suffix.
# The rule's command uses some built-in Make Macros:
# $@ for the pattern-matched target
# $< for the pattern-matched dependency
# -std=c++11 useful in cygwin
# -std=c++0x useful in BeagleBone Black
# -Wall good to display all warnings
%.o : %.cpp
	g++ -g -std=c++0x -Wall -o $@ -c $<

# These are Dependency Rules, which are rules without any command.
# Dependency Rules indicate that if any file to the right of the colon changes,
# the target to the left of the colon should be considered out-of-date.
# The commands for making an out-of-date target up-to-date may be found elsewhere
# (in this case, by the Pattern Rule above).
# Dependency Rules are often used to capture header file dependencies.
baro_first.o : bmp180.h


