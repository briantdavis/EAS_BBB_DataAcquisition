#! /bin/sh
# /etc/init.d/EAS-RV12-Startup
#

# Scipt as follows

### Neccessary Start-up items ###

mkdir /logs
# This will likely emit an error, ignore it. #
# This guarentees that the working directory for this file will exist #

# This variable determines whether the crypt will use the log function or not #
logging=true
debugging=true
delay="2s";   ### MUST BE A POSITIVE INTEGER FOLLOWED BY "s" for seconds ###

### Functions ###

log () {
# This is a generic log function so as to avoid a "D.R.Y." infraction.#
# The function pushes a message to a log file, should it be needed later. #
# If variable "logging", located in "Necessary Start-up Items" is set to false... #
# then this function will still be ran, but its abilities shall be left unutilized.#
	if $logging
	then
		for var_log in "$@"
		do
			if $debugging
			then
				echo "$var_log"
			fi
			echo "$var_log"  >> "/logs/$(basename $0).log"
		done
		echo "\n" >> "/logs/$(basename $0).log"
	fi
}

init () {
# This is a generic initiation function so as to avoid a "D.R.Y." infraction. #
# The function trys to call the scripts requested via string input. #
# I personnaly suggest against this function due to possible user error. #
# The function will run all inputted scripts followed by a delay as defined by "delay" #
	log "beginning init"
	log "beginning execution loop"
	for var_init in $*
	do
		log "Execution delayed by:" "$delay"
		sleep $delay
		log "Executing:" "$var_init"
		$var_init >> "/logs/$(basename $var_init).log" &
		log "Executed:" "$var_init"
	done
	log "end execution loop"
	log "end init"
}

findAndExecute () {
# This is a generic 'search and execute' function so as to avoid a "D.R.Y." infraction. #
# The function tries to locate and execute the files requested via string input #
# THIS FUNCTION DOES NOT HANDLE ERRORS #
# I may chage the "init" to activate in the loop to act as an extra delay. #
	log "begining findAndExecute"
	log "creating init_paths variable"
	init_paths=""
	log "begining execution loop"
	for var_findAndExecute in "$@"
	do
		log "Finding:" "$var_findAndExecute"
		path=$(find / -type f -name $var_findAndExecute)
		log "Located at:" "$path"
		init_paths="$init_paths $path"
	done
	log "end execution loop"
	init $init_paths
	log "end findAndExecute"
}

#init "/etc/init.d/testenv/t1" "/etc/init.d/testenv/t2" "/etc/init.d/testenv/t3" "/etc/init.d/testenv/t4" "/etc/init.d/testenv/t5"

log "begin script" "$(date)"
### rm /logs/ti.txt   ### DEVELOPMENTAL CODE ###
findAndExecute "Enable_third_i2c.sh" ### The following is disabled until futher notice: "beta_daq_test.v2.exe" ###
#findAndExecute "t1" "t2" "t3" "t4" "t5" "t6" "t7" "t8" "t9" "t10"   ### DEVELOPMENTAL CODE ###
log "end script" "$(date)"

