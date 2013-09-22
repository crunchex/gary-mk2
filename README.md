Gary Mk2
========================
Repository for all the software needed for Gary Mk2. Should be cloned onto Gary after Angstrom is already running on him.

Features
========================
* PWM driver (Credit: Texas Instruments, Saad Ahmad http://www.saadahmad.ca)
* Servo/Motor library (Credit: Saad Ahmad http://www.saadahmad.ca)

Installation
========================
This includes everything that needs to be done from the moment an Angstrom systemd-image is installed on the BeagleBone.

Compiling the driver on the BeagleBone
-------------------------------
1) Download & install build prereqs (Angstrom)
````sh
opkg update
opkg upgrade
opkg install kernel-dev
opkg install kernel-headers
opkg install task-native-sdk
opkg git
````

2) Set up build environment and make the module
````sh
cd /usr/src/kernel
make scripts
ln -s /usr/src/kernel /lib/modules/$(uname -r)/build
cd ~
git clone git://SaadAhmad/beaglebone-black-cpp-PWM.git
cd beaglebone-black-cpp-PWM/driver
make
````
Using the compiled files
------------------------
Note you can use the compiled kernel driver file however I'm not sure how easily it will work with different kernel versions. 
At the time I compiled it for v3.8.13 and it seems to work for that. If the provided file doesn't work then you might want to try rebuilding it.

1) First make sure that your beaglebone isn't running any any of the overlays from the current bone_pwm and that the pwm_test module isnt loaded
   A reboot should fix this however if you dont want to reboot you can do the following steps to unload the driver 
````sh
cat /sys/devices/bone_capemgr.<WHATEVER YOUR NUMBER IS HERE>/slots
# Find the slot for any pwm modules
echo -<Slot # for the PWM modules> >  /sys/devices/bone_capemgr.<WHATEVER YOUR NUMBER IS HERE>/slots
# Repeat for all modules

#Once done
modprobe -r pwm_test.ko
````

2) Backup the old pwm_test.ko file
````sh
cd /lib/modules/<kernel version>/kernel/drivers/pwm/
cp pwm_test.ko pwm_test.ko.orig   
````
     
3) Find the built pwm_test.ko file and copy it over to /lib/modules/<kernel version>/kernel/drivers/pwm/
 
4) Copy over the built dtbo (sc_pwm_P*.dtbo) files to /lib/firmware

5) Youre done setting up the files! Do a reboot and you should be set

6) Load in the am33xx_pwm module.
````sh
echo am33xx_pwm >  /sys/devices/bone_capemgr.<WHATEVER YOUR NUMBER IS HERE>/slots
````

 
