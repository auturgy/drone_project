## Introduction  
  
This is server library for my drone project. For multi-platform use, I use boost as core engine.


## Build Environment  

**OS** : OS X EI Capitan (Mac Pro)  
**language** : gcc 5.3.0   
**boost** : 1.6.0  

### boost library (from source)
    
현재(16.03.24)까지는 brew를 통해서 최신 버전 설치가 안되므로, 직접 [홈페이지](https://sourceforge.net/projects/boost/files/boost/1.60.0/)에서 받아서 빌드를 해주어야 한다. 압축풀고 다음 명령어 두방이면 라이브러리가 생성된다.  

	$ ./bootstrap.sh  
	$ ./b2 toolset=darwin  
  
위치를 옮기는 것이 편한데, /usr/local/Cellar/boost/1.60.0로 생성하자.  

	$ mkdir /usr/local/Cellar/boost/1.60.0  
	$ mkdir /usr/local/Cellar/boost/1.60.0/include
  
빌드한 곳에서 인클루드와 라이브러리 이동  

	$ sudo mv ./boost /usr/local/Cellar/boost/1.60.0/include  
	$ sudo mv ./stage/* /usr/local/Cellar/boost/1.60.0/  
  
### boost library (from github)  

예전에 했는데, 잘 되는지 모르겠다.  

	$ brew install https://github.com/Homebrew/homebrew/blob/master/Library/Formula/boost.rb
  
  
## how to build server  
  
if you install c++11 and boost without problem, just write down like this,  

	$ makefile  

That is all you have to do. You might see some logs in the terminal with starting message. 
  
  
## how to test 

there are some unit tests written by python in /python. After running server, you can do test as follow,  

	$ python client.py  
	
I am not good at python, but you can easily do bunch of tests by commenting/uncommenting code in main function in the file.  
  
  
## Trouble Shooting  

### build environment  

To get rid of the problem related to build environment, [Docker](https://www.docker.com/) will be adapted.

Based on my experience, most of problems come from link error in a makefile. In this client library, you have to add belows in makefile in order to avoid build error. 

	LIBS = -lm -ldl - lpthread -lboost_atomic  
	
In addition to it, all boost links should be used without "-mt". For example, you use -lboost_regex instead of -lboost_regex-mt in default raspbian.


## License   
Powered by Jay Kim(or Jong Yoon Kim). Code licensed under an MIT-style License. Documentation licensed under CC BY 4.0.
