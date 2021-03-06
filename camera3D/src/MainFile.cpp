//// 3D stero robot (RoboDog)
// Usage :
//	remark below the desired frame function : taken from '"frameFunctions.h"  '
//  all main constants are set in the file '"working_consts.h"'

// TODO : 
//  26/11/15 -  set working_consts by UI, asspacialy choosing REC or PLAYBACK
//				set correct recordings rate		
//				do calibration !!
// DONE :
//	14/11/15 - performance issue..  -> 25/11/15 - smaller resultion to reduce bus trafic !
//  14/02/16 - stereo calibration seems to work finally. smaller resultution captures are bad for calibration.
// 
// helpers:
//   http://answers.opencv.org/question/9503/how-to-increase-the-quality-of-disparity-map/
//   http://www.pyimagesearch.com/2015/05/25/basic-motion-detection-and-tracking-with-python-and-opencv/
// https://singhgaganpreet.wordpress.com/tag/cvboundingrect-example/
//  navigation ref:
//https://www.youtube.com/watch?v=4Y7zG48uHRo 
// tracking ref by: 
// http://blog.christianperone.com/2015/01/real-time-drone-object-tracking-using-python-and-opencv/ !, add gradient hist/info of the object
// look at : https://www.youtube.com/watch?v=kxsvG4sSuvA 


 /* my  constants and parameters */
// bounds in percent from image size
#define MIN_MOVED_AREA_in_image 33//33//.0//23
#define MAX_MOVED_AREA_in_image 95.0
#define NUM_OF_PIXELS_IN_FRAME	(640.0*480.0)
#define MIN_CURVE_AREA			MIN_MOVED_AREA_in_image/100*NUM_OF_PIXELS_IN_FRAME
#define MAX_CURVE_AREA			MAX_MOVED_AREA_in_image/100*NUM_OF_PIXELS_IN_FRAME
#define SHOW_MOVING_CONTOURS	true//true
#define SHOW_MOVING_RECTANGLES	false//true
#define SHOW_MOVING_BIG_CONTOURS	true
#define SHOW_MOVING_BIG_RECTANGLES	false//true

// below or above those limits it will be treated as noise. (or too close movement to camera)
#include "stereo_functions.hpp"

#include "frameFunctions.h"		// general definitions and functions. that's why it is first to include.
#include "working_consts.h"		// my added definitions, constants
#include "utilFunctions.h"		// utility functions for the application.

#include "BackgroundSub.hpp"

#include "GUI_functions.h"

//#include "externals\objectTrackingTut.cpp"
//#include "externals\multipleObjectTracking.cpp"
//#include "externals\motionTracking_myChanges.cpp"
///#include "externals\motionTracking_ver14.cpp"
////#include "externals\stereoBMTuner-1.0\StereoBMTuner\stereoBMTuner.h"
///-- to do by ver 3.1. : #include "externals\akaze.cpp"  //300416
///#include "C:\opencv3_1\opencv-master\samples\cpp\tutorial_code\features2D\AKAZE_tracking\planar_tracking.cpp"
//#include "C:\opencv3_1\opencv-master\samples\cpp\convexhull.cpp"////////


// also : https://en.wikipedia.org/wiki/Image_moment#Examples_2
#include "camshiftdemo.cpp"

#include "stereo_calib.h"
//#include ".\kalman.cpp"

//
//#include <windows.h>
//using namespace cv;
//using namespace std;

////when need to eliminate the consule that is opened in parallel 
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

void specific_match()
{
	char*	argv2[2];  //6
	//// TODO: put ini a loop to get those from cameras
	argv2[0] = "../run_inputs/stereo_calibration_images/shot_1_011.jpg";  // left image
	argv2[1] = "../run_inputs/stereo_calibration_images/shot_0_011.jpg";  // right image
	Mat imgL = imread(argv2[1], -1);
	Mat imgR = imread(argv2[0], -1);
	Mat outM;
	do_stereo_disp(imgR,imgL,outM);
	imshow("right",imgR);
	imshow("left",imgL);
	imshow("disp out",outM);

	waitKey(0);

	destroyAllWindows();
}

int main(int argc, char** argv) 
{
	//specific_match();  // testing for sending right parameters

	thisStereo.input_source =LIVE_CAM;

	
	show_buttons_gui();
	/*
	initialize_vid_source();
	display_L_R_stream();
	
	return 0;*/

	//ask_user_for_video_source();	// get file name or open camera0 (+1 later for stereo)
	string	base_out_file_path	= "C:/Users/Ran_the_User/Documents/Technion_Studies/IP_STUFF/video_4_testing/out";
	string	framesCounterStr	= ""	, base_file_name = "" , file_full_name="", file_suffix = ".*";	
	int		stream_frame_index	= 0;
	char	rec_file_name[150]  = "C:/Users/Ran_the_User/Documents/Technion_Studies/IP_STUFF/video_4_testing/in/VID_3D_scenario/output_1.avi";
	VideoCapture vid			= VideoCapture(rec_file_name);	

	bool first_setup = true;

	int		loop_delay = 33 ; //[ms]	// need to fit the file recording value
	char	user_pressing=0;	// just optional.

	int relative_counter =0;
	VideoCapture vidR,vidL;

	if( !vid.isOpened() )
		return -1;

	plotWindowsNames[0] = "win1 - right stereo image";
	plotWindowsNames[1] = "win2 - left stereo image"; 
		// TODO: added frame of copmosed stiched image ? can take relative very long process.
			// anyway can add as ref. with other low-prio thred
		//  it is actually part of the disparity process.
	plotWindowsNames[2] = "win3 - calculated disparity";
	plotWindowsNames[3] = "win4 - background substruction output";
	plotWindowsNames[4] = "win5 - tracked object";

	BackSubs localBackSubs ;
	do_stereo_disp_init();

	while (1)
	{
		if (op_flags.make_stereo_calibration)
		{		
			argc = 6;
			argv[1] = "-w";  argv[2] = "8";
			argv[3] = "-h";  argv[4] = "6";
			argv[5] = "../run_inputs/stereo_calibration_images/stereo_calib.xml";
			do_stereo_calib(argc, argv);
			op_flags.make_stereo_calibration	=	false;
		}

		if (op_flags.calc_background_subs)
		{
			//localBackSubs.show_forgnd_and_bgnd();
		}

		if(op_flags.show_stereo)
		{
			relative_counter++;

			if (first_setup) {
				////check of sources already active
				//vidR			= VideoCapture(1);	
				//vidL			= VideoCapture(2);	

				vidR.open(1);	
				vidL.open(2);	
				cvWaitKey(1000); // initial delay for init
				int w=320,	h=240;
				vidR.set(CAP_PROP_FRAME_WIDTH, w);	vidR.set(CAP_PROP_FRAME_HEIGHT, h);
				vidL.set(CAP_PROP_FRAME_WIDTH, w);	vidL.set(CAP_PROP_FRAME_HEIGHT, h);
				first_setup = false;

				localBackSubs.show_forgnd_and_bgnd_init(2); //with Left cam
			}
			vidR >> plotImages[0];
			vidL >> plotImages[1];
			// add check for empty
			imshow(plotWindowsNames[0],  plotImages[0]);
			imshow(plotWindowsNames[1],  plotImages[1]);
			
			localBackSubs.show_forgnd_and_bgnd( plotImages[1]) ;

			if (relative_counter>20){
				do_stereo_disp(plotImages[0+1],plotImages[1*0], plotImages[2]);  // plotImages[2] is the disparity output
				imshow(plotWindowsNames[2],  plotImages[2]);
				relative_counter	=	0;
			}
			//op_flags.show_stereo = false;
		}

		if (op_flags.reset_vid_file_location) // TODO: verify source is file and not camera.
		{
			vid.set(CAP_PROP_POS_FRAMES,0);	// go back to first frame
			op_flags.reset_identification	 = true;
			op_flags.reset_vid_file_location = false;
		}
		if (op_flags.reset_identification){
			reset_camshift_vars();			// a funciton to reset the camshift main variables. 
			op_flags.reset_identification	= false;
		}
		if (op_flags.play_on){

			vid >> plotImages[0];
			if (plotImages[0].empty())
				op_flags.play_on = false;
			else
			{					
				imshow(plotWindowsNames[0], plotImages[0]); 
				if (op_flags.make_camshift)
					do_camshift_on_Single_current_frame(&plotImages[0]); 

				if (op_flags.show_stereo) /// TODO!!
				{
					imshow(plotWindowsNames[1], plotImages[0]);
				//	imshow(plotWindowsNames[2], plotImages[1]);
					///plot_images[2] = calc_disparity();
					/*imshow(plotWindowsNames[3], plotImages[2]);*/

				}
				if (op_flags.show_vid_source_selection)
				{
					
				}
			}


		}
		
		if (!check_user_input(&loop_delay, &user_pressing))
			break;
	}


	return 0;



	do_camshift_main();
	return 0;


//	show_forgnd_and_bgnd();
	return 0;

	/////////////////////////////////////////////////////

	thisStereo.input_source = LIVE_CAM;
	
	show_user_gui();

	waitKey();

	destroyAllWindows();
	return 0;
	/* global partial init */
	//do_track_subject(argc, argv);
	//do_track_mult();
	//main_track();
	//main_14
	//////////////////////////////////////////////////////
}