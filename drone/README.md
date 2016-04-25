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
**[ServoBlaster]([ServoBlaster GitHub](https://github.com/richardghirst/PiBits/tree/master/ServoBlaster))** : WiringPi 개발자가 추전한 Software PWM. 결국 사용하지 않음   
**[pi-blaster](https://github.com/sarfata/pi-blaster)** : PWM output 시그널링의 문제로 servobalster 대신 사용. 헌데, 생각해보면 보낸 출력 값의 잘못으로 전체 PWM이 흔들리것이 아닌가 싶다. but, who knows? 그리고, 설치하는데 반나절을 보냈다. 이유는 설치 설명이 잘못 되어 있다. ㅠ.ㅠ 생고생한 것 생각하면 쩝... 어째든 deb package 만드는 방법으로 해야한다. **주의할 점**이 있는데, 서보셋팅이 default로 16개가 되어있다. GPIO를 괜히 잡아먹기도 하고, 성능이 열화되기 때문에 빌드 전에 **안쓰는 GPIO는 주석 처리** 하도록 하다.  


```
$ sudo apt-get update & upgrade 
$ sudo apt-get git 
$ sudo apt-get install libboost-all-dev
$ sudo apt-get install vim (option)

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
