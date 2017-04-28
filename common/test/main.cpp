#ifndef NOT_OPEN_CV_USE

#include <iostream>
#include <fstream>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <time.h>
#include <dirent.h>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

// Check if the directory exists, if not create it
// This function will create a new directory if the image is the first
// image taken for a specific day
inline void directoryExistsOrCreate(const char* pzPath)
{
    DIR *pDir;
    // directory doesn't exists -> create it
    if ( pzPath == NULL || (pDir = opendir (pzPath)) == NULL)
        mkdir(pzPath, 0777);
    // if directory exists we opened it and we
    // have to close the directory again.
    else if(pDir != NULL)
        (void) closedir (pDir);
}

// When motion is detected we write the image to disk
//    - Check if the directory exists where the image will be stored.
//    - Build the directory and image names.
int incr = 0;
inline bool saveImg(Mat image, const string DIRECTORY, const string EXTENSION, const char * DIR_FORMAT, const char * FILE_FORMAT)
{
    stringstream ss;
    time_t seconds;
    struct tm * timeinfo;
    char TIME[80];
    time (&seconds);
    // Get the current time
    timeinfo = localtime (&seconds);
    
    // Create name for the date directory
    strftime (TIME,80,DIR_FORMAT,timeinfo);
    ss.str("");
    ss << DIRECTORY << TIME;
    directoryExistsOrCreate(ss.str().c_str());
    ss << "/cropped";
    directoryExistsOrCreate(ss.str().c_str());

    // Create name for the image
    strftime (TIME,80,FILE_FORMAT,timeinfo);
    ss.str("");
    if(incr < 100) incr++; // quick fix for when delay < 1s && > 10ms, (when delay <= 10ms, images are overwritten)
    else incr = 0;
    ss << DIRECTORY << TIME << static_cast<int>(incr) << EXTENSION;
    return imwrite(ss.str().c_str(), image);
}

// Check if there is motion in the result matrix
// count the number of changes and return.
inline int detectMotion(const Mat & motion, Mat & result, Mat & result_cropped,
                 int x_start, int x_stop, int y_start, int y_stop,
                 int max_deviation,
                 Scalar & color)
{
    // calculate the standard deviation
    Scalar mean, stddev;
    meanStdDev(motion, mean, stddev);
    // if not to much changes then the motion is real (neglect agressive snow, temporary sunlight)
    if(stddev[0] < max_deviation)
    {
        int number_of_changes = 0;
        int min_x = motion.cols, max_x = 0;
        int min_y = motion.rows, max_y = 0;
        // loop over image and detect changes
        for(int j = y_start; j < y_stop; j+=2){ // height
            for(int i = x_start; i < x_stop; i+=2){ // width
                // check if at pixel (j,i) intensity is equal to 255
                // this means that the pixel is different in the sequence
                // of images (prev_frame, current_frame, next_frame)
                if(static_cast<int>(motion.at<uchar>(j,i)) == 255)
                {
                    number_of_changes++;
                    if(min_x>i) min_x = i;
                    if(max_x<i) max_x = i;
                    if(min_y>j) min_y = j;
                    if(max_y<j) max_y = j;
                }
            }
        }
        if(number_of_changes){
            //check if not out of bounds
            if(min_x-10 > 0) min_x -= 10;
            if(min_y-10 > 0) min_y -= 10;
            if(max_x+10 < result.cols-1) max_x += 10;
            if(max_y+10 < result.rows-1) max_y += 10;
            // draw rectangle round the changed pixel
            Point x(min_x,min_y);
            Point y(max_x,max_y);
            Rect rect(x,y);
            Mat cropped = result(rect);
            cropped.copyTo(result_cropped);
            rectangle(result,rect,color,1);
        }
        return number_of_changes;
    }
    return 0;
}

int main (int argc, char * const argv[])
{
    const string DIR = "/home/pi/motion_src/pics/"; // directory where the images will be stored
    const string EXT = ".jpg"; // extension of the images
    const int DELAY = 500; // in mseconds, take a picture every 1/2 second
    const string LOGFILE = "/home/pi/motion_src/log";

    // Format of directory
    string DIR_FORMAT = "%d%h%Y"; // 1Jan1970
    string FILE_FORMAT = DIR_FORMAT + "/" + "%d%h%Y_%H%M%S"; // 1Jan1970/1Jan1970_12153
    string CROPPED_FILE_FORMAT = DIR_FORMAT + "/cropped/" + "%d%h%Y_%H%M%S"; // 1Jan1970/cropped/1Jan1970_121539

    // Set up camera
    CvCapture * camera = cvCaptureFromCAM(CV_CAP_ANY);
    cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, 1280); // width of viewport of camera
    cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, 720); // height of ...
    
    // Take images and convert them to gray
    Mat result, result_cropped;
    Mat prev_frame = result = cvQueryFrame(camera);
    Mat current_frame = cvQueryFrame(camera);
    Mat next_frame = cvQueryFrame(camera);
    cvtColor(current_frame, current_frame, CV_RGB2GRAY);
    cvtColor(prev_frame, prev_frame, CV_RGB2GRAY);
    cvtColor(next_frame, next_frame, CV_RGB2GRAY);
    
    // d1 and d2 for calculating the differences
    // result, the result of and operation, calculated on d1 and d2
    // number_of_changes, the amount of changes in the result matrix.
    // color, the color for drawing the rectangle when something has changed.
    Mat d1, d2, motion;
    int number_of_changes, number_of_sequence = 0;
    Scalar mean_, color(0,255,255); // yellow
    
    // Detect motion in window
    int x_start = 10, x_stop = current_frame.cols-11;
    int y_start = 350, y_stop = 530;

    // If more than 'there_is_motion' pixels are changed, we say there is motion
    // and store an image on disk
    int there_is_motion = 5;
    
    // Maximum deviation of the image, the higher the value, the more motion is allowed
    int max_deviation = 20;
    
    // Erode kernel
    Mat kernel_ero = getStructuringElement(MORPH_RECT, Size(2,2));
    
    // All settings have been set, now go in endless loop and
    // take as many pictures you want..
    while (true){
        // Take a new image
        prev_frame = current_frame;
        current_frame = next_frame;
        next_frame = cvQueryFrame(camera);
        result = next_frame;
        cvtColor(next_frame, next_frame, CV_RGB2GRAY);

        // Calc differences between the images and do AND-operation
        // threshold image, low differences are ignored (ex. contrast change due to sunlight)
        absdiff(prev_frame, next_frame, d1);
        absdiff(next_frame, current_frame, d2);
        bitwise_and(d1, d2, motion);
        threshold(motion, motion, 35, 255, CV_THRESH_BINARY);
        erode(motion, motion, kernel_ero);
        
        number_of_changes = detectMotion(motion, result, result_cropped,  x_start, x_stop, y_start, y_stop, max_deviation, color);
        
        // If a lot of changes happened, we assume something changed.
        if(number_of_changes>=there_is_motion)
        {
            if(number_of_sequence>0){ 
                saveImg(result,DIR,EXT,DIR_FORMAT.c_str(),FILE_FORMAT.c_str());
                saveImg(result_cropped,DIR,EXT,DIR_FORMAT.c_str(),CROPPED_FILE_FORMAT.c_str());
            }
            number_of_sequence++;
        }
        else
        {
            number_of_sequence = 0;
            // Delay, wait a 1/2 second.
            cvWaitKey (DELAY);
        }
    }
    return 0;    
}


#endif /* NOT_OPEN_CV_USE */


#ifdef NOT_TEST_PUBLIC_IP
/////////////////////////////////////////////////////////////////////////////
//
//  retreive public IP in NAT envirionment to comunicate p2p or cs
//
//  1. 클라이언트는 gethostname으로 사설 or 공인 IP의 주소를 저장함.
//  2. NAT환경이 아닌 게임서버 혹은 스턴서버에게 dummy Packet을 보냄. 
//  3. 서버는 받은 패킷의 IP, PORT로 IP, PORT를 패킷 내용에 담아 클라이언트에게 보냄.
//  4. 클라이언트는 사설 IP, 공인 IP 모두 저장.
//  5. 클라이언트는 수집된 사설 IP, 공인 IP를 서버에게 보냄.
//  6. 파티가 이루어질 경우 서버는 각각의 Peer에게 상대방의 사설,공인 IP를 전송.
//  7. 각각의 Peer들은 일정시간동안 사설 IP로 패킷 전송.
//  8. 사설 IP로 패킷 전송 실패시 공인 IP로 패킷 전송.
//  9. 공인 IP로 패킷 전송 실패시 최후의 보루 릴레이서버에게 맡김.
//
//                                      from http://202psj.tistory.com/840
//
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int   main( void)
{
    char            host_name[80];
    struct hostent *host_entry;
    int             ndx;
                 
    if ( 0 != gethostname( host_name, sizeof( host_name)))
    {
        printf( "gethostname() failed/n");
        exit( 1);
    }                 
    
    printf( "%s\n", host_name);
    
    host_entry = gethostbyname( host_name);

    if ( !host_entry)
    {
        printf( "gethostbyname() failed/n");
        exit( 1);
    }

    for ( ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++)
        printf( "%s\n", inet_ntoa( *(struct in_addr*)host_entry->h_addr_list[ndx]));

    return 0;
}

#endif /* NOT_TEST_PUBLIC_IP */