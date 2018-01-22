#! /bin/sh
# Scipt as follows
### BEGIN INIT INFO
# Provides:		EAS RV12 DataAquisition System
# Required-Start: 	$all
# Required-Stop:
# Default-Start:	2 3 4 5
# Default-Stop:		0 1 6
# Short-Description:	EAS-RV12-Startup Script
### END INIT INFO

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
			echo "$var_log"  >> "/EAS/logs/$(basename $0).log"
		done
		echo "\n" >> "/EAS/logs/$(basename $0).log"
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
		$var_init >> "/EAS/logs/$(basename $var_init).log" &
		log "Executed:" "$var_init"
	done
	log "end execution loop"
	log "end init"
}

findAndExecute () {
### Deprecated ###
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

execute_code () {
	mkdir /EAS/logs
	logging=true
	bebugging=true
	delay="2s"
	log "begin script" "$(date)"
	init "/EAS/boot/EAS-RV12-Init/EAS-RV12-Startup-Leds.sh" "/EAS/boot/Enable_third_i2c.sh"
	log "end script" "$(date)"
}

kill_code () {
	init "/EAS/boot/EAS-RV12-Init/EAS-RV12-Shutdown-Leds.sh"
}

case "$1" in
	start)
		echo -n "Starting EAS-RV12 Data Aquisition System"
		/usr/sbin/EAS-RV12-Startup.sh start
		execute_code
		;;
	stop)
		echo -n "Stopping EAS-RV12 Data Aquisition System"
		/usr/sbin/EAS-RV12-Startup.sh stop
		kill_code
		;;
	restart)
		echo -n "Restarting EAS-RV12 Data Aquisition System"
		/usr/sbin/EAS-RV12-Startup.sh restart
		;;
	*)
		echo "Usage: $0 {start|stop|restart}"
		exit 1;
		;;
esac
