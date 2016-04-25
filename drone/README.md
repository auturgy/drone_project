## Introduction  
  
This is the daemon run on Raspberry pi on drone. The primary purpose is to send PWM signal to flight controller so as to contorl drone's position and movement. Secondly, it forwads MAVlink coming from flight controller to a ground station through UDP. Lastly, it also has possibilities to supprt an additional functionality such as video streaming, lidar-lite or other sensors.  

## how to deploy on Raspberry Pi II/III
  
### Raspbian 설치    
  
  - [공식사이트](https://www.raspberrypi.org/downloads/raspbian/)에서 라즈비안 다운로드  
  - [설치매뉴얼/맥PC용](https://www.raspberrypi.org/documentation/installation/installing-images/mac.md)에 설명한 방법으로 설치   
 
### Raspbian Lite 버전일 경우 
  - default ID/PW는 pi/raspberry
  - sudo raspi-config 로 **파일시스템 확장(전체 SD카드 사용으로 변경)**, 시간대, 키보드, 패스워드 변경 
  - sudo vi /etc/wpa_supplicant/wpa_supplicant.conf 로 와이파이 설정  (연결은 sudo apt-get update로 확인)
  
```  
network = { 
    ssid="접속할 AP ID"
    psk="비밀번호"
}  
```  
### Build Environment  
  
**OS** : raspbian ([NOOBS v1.8.0](https://www.raspberrypi.org/downloads/noobs/))  
**Language** : GNU c++14  
**Boost** : 1.5.5  
**[ServoBlaster]([ServoBlaster GitHub](https://github.com/richardghirst/PiBits/tree/master/ServoBlaster))** : WiringPi 개발자가 추전한 Software PWM.   
**[pi-blaster](https://github.com/sarfata/pi-blaster)** : servobalster 대안으로 사용 가능하나, 테스트하지 않음. **주의할 점**이 있는데, 서보셋팅이 default로 16개가 되어있다. GPIO를 괜히 잡아먹기도 하고, 성능이 열화되기 때문에 빌드 전에 **안쓰는 GPIO는 주석 처리** 하도록 하다.  

> **CC3D** FC의 경우, RPI의 Software PWM을 잘 인식 못하는 문제가 있는데, LibrePilot로 Firmware 업데이트를 하면 해결할 수 있다. 가능하면, H/W PWM보드를 붙이는 것이 좋아 보인다. [참고사이트](http://robotics.stackexchange.com/questions/8965/cc3d-replacing-rc-emitter-with-an-rpi)  
  

```
$ sudo apt-get update & upgrade 
$ sudo apt-get git 
$ sudo apt-get install libboost-all-dev
$ sudo apt-get install vim (option)

<- install servoblaster ->  
$ git clone https://github.com/richardghirst/PiBits 
$ cd PiBits/servoblaster/user 
$ sudo make install

<- install pi-blaster ->  
$ sudo apt-get install debhelper dh-autoreconf dh-systemd dpkg-dev init-system-helpers autoconf  
$ git clone https://github.com/sarfata/pi-blaster.git  
$ dpkg-buildpackage -us -uc -i  
$ sudo dpkg -i ../pi-blaster*.deb  
  

```  

> 공인 IP 확인 방법 : 
> curl ifconfig.me

## 드론 조립 및 개발  

TBD  

## Trouble Shooting  

### build environment  

To get rid of this problem, [Docker](https://www.docker.com/) will be adapted. However, this will be needed if you are not familiar with docker. 

Based on my experience, most of problems come from link error in makefiles. In this client library, you have to add belows in makefile in order to avoid build error. 

	LIBS = -lm -ldl - lpthread -lboost_atomic  
	
In addition to it, all boost links should be used with "-mt". For example, you use -lboost_regex instead of -lboost_regex-mt in default raspbian.
  
## Reference Sites  

[http://bakyeono.net/post/2015-08-30-connect-to-wifi-and-bluetooth-keyboard-on-raspberry-pi.html](http://bakyeono.net/post/2015-08-30-connect-to-wifi-and-bluetooth-keyboard-on-raspberry-pi.html)  
[https://solarianprogrammer.com/2015/01/13/raspberry-pi-raspbian-install-gcc-compile-cpp-14-programs/](https://solarianprogrammer.com/2015/01/13/raspberry-pi-raspbian-install-gcc-compile-cpp-14-programs/)  
[http://hertaville.com/2012/11/18/introduction-to-accessing-the-raspberry-pis-gpio-in-c/](http://hertaville.com/2012/11/18/introduction-to-accessing-the-raspberry-pis-gpio-in-c/)  
[servoblaster 설치 참고](http://cosmosjs.blog.me/220665844005)  
[http://www.microchip.com/forums/m154353.aspx](http://www.microchip.com/forums/m154353.aspx)


## License   
Powered by Jay Kim(or Jong Yoon Kim). Code licensed under an MIT-style License. Documentation licensed under CC BY 4.0.
