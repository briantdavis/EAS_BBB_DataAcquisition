#
# EAS Project
#
# Embry-Riddle Aeronautical University - Prescott Campus
# B.Davis
#
# Makefile.

RP3_TARGET = /EAS/bin/eas_daq_rp3.v2.exe

BBB_TARGET = /EAS/bin/eas_daq_bbb.v2.exe

PAN_TARGET = /EAS/bin/misc/pan_flute.exe 

# Here is a Make Macro that uses the backslash to extend to multiple lines. 
COMMON_OBJS =  \
  eas_daq_pack.o \
  bmp180.o \
  bme280.o \
  ADXL345Accelerometer.o \
  pca9544mux.o \
  hscPress.o \
  mpu6050AccelGyro.o
  
RP3_OBJS = rpi3_eas_daq.o

BBB_OBJS = bbb_eas_daq.o

PAN_OBJS = pan_flute.o

# Here is a Make Macro defined by two Macro Expansions.
# A Macro Expansion may be treated as a textual replacement of the Make Macro.
# Macro Expansions are introduced with $ and enclosed in (parentheses).
REBUILDABLES = $(COMMON_OBJS) $(RP3_OBJS) $(BBB_OBJS) $(LINK_TARGET)

# The rule for "all" is used to incrementally build your system.
# It does this by expressing a dependency on the results of that system,
# which in turn have their own rules and dependencies.
all : $(BBB_TARGET)
	echo All done

#rp : $(RP3_TARGET)
#  echo RP done

pan_flute : $(PAN_TARGET)
	echo pan_flute done

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
$(RP3_TARGET) : $(COMMON_OBJS) $(RP3_OBJS) ./libs/tc/tinycon.o
	g++ -g -Wall -lrt -o $@ $^

# Here is a Rule that uses some built-in Make Macros in its command:
# $@ expands to the rule's target, in this case "test_me.exe".
# $^ expands to the rule's dependencies, in this case the three files
# main.o, test1.o, and  test2.o.
$(BBB_TARGET) : $(COMMON_OBJS) $(BBB_OBJS) ./libs/tc/tinycon.o
	g++ -g -Wall -lrt -o $@ $^

# Here is a Rule that uses some built-in Make Macros in its command:
# $@ expands to the rule's target, in this case "test_me.exe".
# $^ expands to the rule's dependencies, in this case the three files
# main.o, test1.o, and  test2.o.
$(PAN_TARGET) : $(COMMON_OBJS) $(PAN_OBJS) ./libs/tc/tinycon.o
	g++ -g -Wall -lrt -o $@ $^
	
#
# Specific Rules
#./libs/tc/tinycon.o : ./libs/tc/tinycon.cpp
#  g++ -g -std=c++0x -o $@ -c $<
  
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

