## Introduction  

This application run on OS X or other OS on laptop, because it provides multi-platform build. The operation is very simple, in which RC signal is collected by Arduino UNO and sent to this application which has TCP/IP connection with module on drone so as to handle drone itself. It could be delayed due to its characteristics of TCP/IP, so I need to consider UDP connection alternatively.


## RCS Collector  
  
The firmare is uploaded on arduino UNO which is connected with rc recevier, FS-iA6. There is no something that makes RC Signal stable when collected, and it needs to have an additional circuit that I do not know well. The receiver provides 6 channels to control, but you only use 4 channels in this project. To minimize development range, it just sends RC signals to MAC OS (or other laptop) through serial port.  


## Reference Sites  
  
- [https://gist.github.com/yoggy/3323808](https://gist.github.com/yoggy/3323808)
- [http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/](http://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/)   
  
## License   
Powered by Jay Kim(or Jong Yoon Kim). Code licensed under an MIT-style License. Documentation licensed under CC BY 4.0.
