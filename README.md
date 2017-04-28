## Introduction  


## Development Environment  

There are several technology used in this project, and you may be frustrated if you do not install what I describe below. :)

### c++  

Default c++ I am using here is c++14 called modern c++ and its version is above 5.0. Do not worry about it if you are not familiar with. google helps you very much.     

### Boost    

all c++ applications in this project are based on boost library and and are independent on specific version of boost.  
  
**Dynamic Library Path :** In some environment like my macbook, you can not execute the app even if I successfully compile it, because the error comes out with the message below,

	dyld: Library not loaded: XXXXXXX 
	
I figure out that it happens when your terminal does not have libarary path, so you need to add following line into your ~/.bash_profile.  

	vim ~/.bash_profile  
	export DYLD_LIBRARY_PATH= /usr/local/Cellar/boost/XXXX  
  	
**Installation on osx :** The best way to install is to use "brew". It is simple and no needs to spend your precious time. :)  

	brew search boost <- show all boost related stuffs
	brew install boost <- default  
	
**makefile modification :** Each application written in c++ has makefile only for my environment. You need to change a couple of things. 

	CC = g++-5
	LIBS = -L/usr/local/Cellar/boost/1.60.0_2/lib
	INCLUDES = -I/usr/local/Cellar/boost/1.60.0_2/include/  
	
## Raspberry Pi  

I am not a big fan of this open source hardware, because performance is relatively poor compared to others such as ordroid series. I strongly recommend for you to consider other boards if you have enough money and time.  

### Raspbian insatllation  

**Download :** From [official site](https://www.raspberrypi.org/downloads/raspbian/) you can get the latest version raspbian and need to unzip it.    

**SD card format :** with SDForamt app, you can format a sd card for installing raspbian.    

**Flashing image :** there are 3 steps to flash. 

	diskutil list  <= disk name of your sd card formatted  
	diskutil unmountDisk /dev/disk2 <= unmount
	sudo dd bs=1m if=2016-05-10-raspbian-jessie.img of=/dev/rdisk2 <= flashing the image downloaded  

**Korean Support :** [사이트](http://blog.naver.com/6533574/220709666224) 참고  

		
	

### Pi Camera 

This project basically does not require any camera capbility. It just shows how it works on air while a drone is flying. Perhaps, stereo camera may be needed in the future so as to implement collision avoidance.  

**Where to buy :** I really like aliexpress, and it offers clone pi camera only with [$21.24](http://www.aliexpress.com/item/New-Camera-Module-Board-5MP-160-Degree-Wide-Angle-Fish-Eye-Lenses-For-Raspberry-Pi/32375046422.html?spm=2114.30010308.3.174.Bd88UV&ws_ab_test=searchweb201556_0,searchweb201602_3_10017_10021_507_10022_10020_10009_10008_10018_10019_101,searchweb201603_2&btsid=0cd8b287-433e-42f9-bbef-bce917d18a6c). How amazing it is. :)  

**How to enable :** You need to attach it into camera slot on board. and enable camera configuration with raspi-config 

	sudo raspi-config  
	-> Choose "6 Enable Camear"  
	-> Select "Enable"
	-> Then, reboot  

**V4L2 driver :** linux standard device for camera  

	sudo modprobe bcm2835-v4l2
  
**Camera Test :** Simply you can test it without installing an additional application. 

	raspistill -o filename.jpg
	raspistill -vf -hf -o filename.jpg -> when your cam is placed upside down
	raspivid -o video.h264
  
### GStreamer  

This libary is the most famous in multimedia industry. I think you can use it for your comercial service just with litte modification.  

**OSX :** I tried to use an another application like VLC and there was a few minutes to figure out that I needed to install gstreamer on my mac book. Fortunately, "brew" provides all you need. :)   

	brew install gstreamer gst-libav gst-plugins-ugly gst-plugins-base gst-plugins-bad gst-plugins-good  

### live555  

Another famous streaming library. One of my colleagues recommends it and I decided to try this as an alternative.  

### Raspberry Pi 3 as a companion computer  

[ardupilot site](http://ardupilot.org/dev/docs/raspberry-pi-via-mavlink.html) explains how to communicate with RPI2 ,**not** RPI3. Therefore, I need to figure out the difference in order to work properly with RPI3. Please make sure that you read whole page of the site first before doing below instructions.   

**Envirionment setting :** You need additional python libraries which are not mentioned on the site. 
  
	sudo apt-get update
	sudo apt-get install python-matplotlib python-serial python-wxgtk2.8 python-lxml
	sudo apt-get install python-scipy python-opencv ccache gawk git python-pip python-pexpect  
	sudo apt-get install screen python-dev libxml2-dev libxslt-dev
	
	sudo pip install pymavlink
	sudo pip install mavproxy  
  
**Serial port enable :** On the contrary to the site, serial port on RPI3 needs to be enabled by using "raspi-config"

**/dev/ttyS0** is used for serial communication between Pixhawk and RPI3. To check it, just type "mavproxy.py" that automatically detects an appropriate port to deal with. You may be able to see some logs. 


## References  
  
[Boost Library on OSX - dyld Woes](http://faisalzqureshi.blogspot.kr/2014/01/boost-library-on-osx-dyld-woes.html)  
[RASPBERRY PI CAMERA BOARD – GSTREAMER](http://blog.gbaman.info/?p=150)  
[파이카메라 설정 및 사용](http://blog.naver.com/hj_0317/220629027995)  
[gstreamer error on mac](http://stackoverflow.com/questions/28385540/gstreamer-error-on-mac)  
[Raspberry Pi 3 Serial Port Usage](https://www.abelectronics.co.uk/kb/article/1035/raspberry-pi-3-serial-port-usage)




	 