// ----------------------------------------------------------------------------
// Trivial Linux serial driver class

// Copyright (C) A Huaman
// Copyright (C) Hazim Bitar
// Copyright (C) 2013  Embecosm Limited <info@embecosm.com>

// Contributor A Huaman
// Contributor Hazim Bitar
// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

// The original version of this code was written by A Huaman, based on the
// classic facedetect.cpp in the OpenCV samples/c directory. Subsequently
// remixed by Hazim Bitar for the Instructables face detection
// project. Neither included a license header in the code, but the whole
// OpenCV project is licensed under the Berkeley Systems Development (BSD)
// license, so this must be presumed to apply to both.

// This version is licensed under the GPL, which is compatible (in this
// direction) with BSD. Changes include the serial header used (suitable for
// Linux), formatting to GNU standard, using a command line argument to
// identify the device and specifying the camera number as -1 (any camera).

/**
 * Original comment:
 *
 * @file objectDetection.cpp
 * Remixed by Hazim Bitar
 * Based on code writtenby A. Huaman ( based in the classic facedetect.cpp in
 * samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade
 * classifier and how to find objects (Face + eyes) in a video stream
 * Mdified to intercept X,Y of center of face object and sends them to Arduino
 * Uno via serial
 */

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
  
#include <iostream>
#include <cstdio>
#include "Eserial.h"

using namespace std;
using namespace cv;
 
/** Function Headers */ 
void detectAndDisplay (Mat frame);
 
/** Global variables */ 
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Captcha";
 
// Serial to Arduino global declarations
int arduino_command;
Eserial * arduino_com;
short MSBLSB = 0;
unsigned char MSB = 0;
unsigned char LSB = 0;

// Serial to Arduino global declarations
int
main (int argc, const char **argv) 
{
  CvCapture * capture;
  Mat frame;
  
  if (argc != 2)
    {
      cerr << "Usage: techbitarFaceDetection <serial device>" << endl;
      exit (1);
    }

  // serial to Arduino setup 
  arduino_com = new Eserial ();
  if (arduino_com != 0)
    {
      //Standard Arduino uses /dev/ttyACM0 typically, Shrimping.it
      //seems to use /dev/ttyUSB0 or /dev/ttyUSB1.
      arduino_com->connect(argv[1], 57600, spNONE);
    }
  
  // serial to Arduino setup 
    
  //-- 1. Load the cascades
  if (!face_cascade.load (face_cascade_name))
    {
      printf ("--(!)Error loading\n");
      return -1;
    };
  if (!eyes_cascade.load (eyes_cascade_name))
    {
      printf ("--(!)Error loading\n");
      return -1;
    };
   
  //-- 2. Read the video stream
  capture = cvCaptureFromCAM (-1);

  if (capture)
    {
      while (true)
	{
	  frame = cvQueryFrame (capture);
	  //-- 3. Apply the classifier to the frame
	  if (!frame.empty ())
	    {
	      detectAndDisplay (frame);
	    }
	  else
	    {
	      printf (" --(!) No captured frame -- Break!");
	      break;
	    }
	  int c = waitKey (10);
	  if ((char) c == 'c')
	    {
	      break;
	    }
	}
    }
  
  // Serial to Arduino - shutdown
  arduino_com->disconnect ();
  delete arduino_com;
  arduino_com = 0;
  
  // Serial to Arduino - shutdown
  return 0;
}

 
/**
 * @function detectAndDisplay
 */ 
  void
detectAndDisplay (Mat frame) 
{
  std::vector < Rect > faces;
  Mat frame_gray;
  cvtColor (frame, frame_gray, CV_BGR2GRAY);
  equalizeHist (frame_gray, frame_gray);
  
  //-- Detect faces
  face_cascade.detectMultiScale (frame_gray, faces, 1.1, 2,
				 0 | CV_HAAR_SCALE_IMAGE, Size (30, 30));
  for (int i = 0; i < faces.size (); i++)
    {
      Point center (faces[i].x + faces[i].width * 0.5,
		    faces[i].y + faces[i].height * 0.5);
      ellipse (frame, center,
	       Size (faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360,
	       Scalar (255, 0, 255), 2, 8, 0);
      
      //  cout << "X:" << faces[i].x  <<  "  y:" << faces[i].y  << endl;
      
      // send X,Y of face center to serial com port      
      // send X axis
      // read least significant byte 
      LSB = faces[i].x & 0xff;
      
      // read next significant byte 
      MSB = (faces[i].x >> 8) & 0xff;
      arduino_com->sendChar (MSB);
      arduino_com->sendChar (LSB);
      cout << "X MSB: " << hex << (int) MSB << ", X LSB: " << (int) LSB << endl;
       
      // Send Y axis
      LSB = faces[i].y & 0xff;
      MSB = (faces[i].y >> 8) & 0xff;
      arduino_com->sendChar (MSB);
      arduino_com->sendChar (LSB);
      cout << "Y MSB: " << hex << (int) MSB << ", Y LSB: " << (int) LSB << endl;
      
      // serial com port send  
      Mat faceROI = frame_gray (faces[i]);
      std::vector < Rect > eyes;
    }  
  //-- Show what you got
  imshow (window_name, frame);
} 
