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


## License   
Powered by Jay Kim(or Jong Yoon Kim). Code licensed under an MIT-style License. Documentation licensed under CC BY 4.0.
