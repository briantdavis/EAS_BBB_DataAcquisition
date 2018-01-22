File: EAS-RV12-Startup.sh

Purpose: Visually notifies user of startup script use and runs some start up routines

How to implement (stepwise):
1. copy file listed above to "/etc/init.d"
2. make the script executable by running the command: "chmod 755 /etc/init.d/EAS-RV12-Startup.sh"
3. register the script to be run at startup: "sudo update-rc.d EAS-RV12-Startup.sh defaults"

This should work, but needs to be tested for verification.