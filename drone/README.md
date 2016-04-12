## Introduction  

## how to deploy on Raspberry Pi II/III
  
### 라즈비안 설치    
  
  - [공식사이트](https://www.raspberrypi.org/downloads/raspbian/)에서 라즈비안 다운로드  
  - [설치매뉴얼/맥PC용](https://www.raspberrypi.org/documentation/installation/installing-images/mac.md)에 설명한 방법으로 설치   
**[Lite 버전 작업]**
  - ID/PW는 pi/raspberry
  - sudo raspi-config 로 **파일시스템 확장**, 시간대, 키보드 , 패스워드 변경 
  - sudo vi /etc/wpa_supplicant/wpa_supplicant.conf 로 와이파이 설정  (연결은 sudo apt-get update로 확인)
  
```  
network = { 
    ssid="접속할 AP ID"
    psk="비밀번호"
}  
```  
### Build Environment  

```
$ sudo apt-get update & upgrade 
$ sudo apt-get git 
$ sudo apt-get install libboost-all-dev
$ sudo apt-get install vim (option)
```  

> 공인 IP 확인 방법 : 
> curl ifconfig.me

## 드론 조립 및 개발  

## Introduction  
  
This is client library for my drone project. For multi-platform use, I use Boost as core engine.

## Build Environment  

**OS** : raspbian ([NOOBS v1.8.0](https://www.raspberrypi.org/downloads/noobs/))  
**Language** : GNU c++11  
**Boost** : 1.5.5

> apt-get install boost  

## Trouble Shooting  

### build environment  

To get rid of this problem, [Docker](https://www.docker.com/) will be adapted. However, this will be needed if you are not familiar with docker. 

Based on my experience, most of problems come from link error in makefiles. In this client library, you have to add belows in makefile in order to avoid build error. 

	LIBS = -lm -ldl - lpthread -lboost_atomic  
	
In addition to it, all boost links should be used with "-mt". For example, you use -lboost_regex instead of -lboost_regex-mt in default raspbian.

## Reference Sites  

[http://bakyeono.net/post/2015-08-30-connect-to-wifi-and-bluetooth-keyboard-on-raspberry-pi.html](http://bakyeono.net/post/2015-08-30-connect-to-wifi-and-bluetooth-keyboard-on-raspberry-pi.html)  
[https://solarianprogrammer.com/2015/01/13/raspberry-pi-raspbian-install-gcc-compile-cpp-14-programs/](https://solarianprogrammer.com/2015/01/13/raspberry-pi-raspbian-install-gcc-compile-cpp-14-programs/)  


## License   
Powered by Jay Kim(or Jong Yoon Kim). Code licensed under an MIT-style License. Documentation licensed under CC BY 4.0.
