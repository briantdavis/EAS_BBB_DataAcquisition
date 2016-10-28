#! /bin/sh

up () {
	echo 1 > brightness
}

down () {
	echo 0 > brightness
}

usr0="/sys/class/leds/beaglebone:green:usr0"
usr1="/sys/class/leds/beaglebone:green:usr1"
usr2="/sys/class/leds/beaglebone:green:usr2"
usr3="/sys/class/leds/beaglebone:green:usr3"
i=0
for var in $(seq 60)
do
	if [ $i -eq 0 ]
	then
		cd $usr1
		down
		cd $usr0
		up
	elif [ $i -eq 1 ]
	then
		cd $usr0
		down
		cd $usr1
		up
	elif [ $i -eq 2 ]
	then
		cd $usr1
		down
		cd $usr2
		up
	elif [ $i -eq 3 ]
	then
		cd $usr2
		down
		cd $usr3
		up
	elif [ $i -eq 4 ]
	then
		cd $usr2
		up
		cd $usr3
		down
	else
		cd $usr2
		down
		cd $usr1
		up
		i=$(expr $i - 6)
	fi
	i=$(expr $i + 1)
	sleep 0.05
done

cd $usr1
down
echo mmc0 > trigger
cd $usr2
echo cpu0 > trigger
cd $usr3
echo mmc1 > trigger
cd $usr0
echo heartbeat > trigger
