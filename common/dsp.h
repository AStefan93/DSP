/** \file dsp.h
 *  \brief Digital Signal Processing Header.
 *
 *  This file includes all the other resources the DSP project requires.
 */
#ifndef DSP_H
#define DSP_H

//Default OpenCV headers
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//Pthread is used for the multithreading applcation.
#include <pthread.h>

//Used for debugging purposes only
//TODO: Add a preprocessing switch in order to remove these if it's for Release purposes.
#include <stdio.h> // Used for printf.
#include <iostream> // Standard input-output library.
#include <ctime> // For time function.

//External submodules headers.
#include <LaneDetection.h> //LaneDetection project.
//#include <ObjectDetection.h> // ObjectDetection project.

//Headers for geometrical transformation and perspective functions.
#include <perspective.h>

//Typedef header used throughout the project.
#include <dsp_typedef.h>


/// Define to specify if the input is a video or a folder with frames. One means video is used.
#define USE_VIDEO (0)



/** \brief Lane Detection Thread Function.
 *
 *  This function is supposed to be called in parallel with Object Detection.
 *  \param ptr_image is a pointer to the image matrix of the current frame
 */
void *LaneDetection_Thread( void * );


/** \brief Object Detection Thread Function.
 *
 *  This function is supposed to be called in parallel with Lane Detection.
 *  \param ptr_image is a pointer to the image matrix of the current frame
 */
void *ObjectDetection_Thread( void * );



namespace DSP{


/** \brief Draw Lanes is an LD Drawing Function.
 *
 *  This function draws over only the inner and outer edges of the detected markers.
 *  \param [in] frame is a pointer to the image matrix of the current frame.
 *  \param [in] corridor is the array with coefficients of the corridor.
 *  \param [in] thickness is the thickness in pixels of the drawn lines.
 */
    void DrawLanes(cv::Mat* ,cv::Mat, cv::Scalar, unsigned int );



/** \brief Fill Lanes is an LD Drawing Function.
 *
 *  This function draws over the original frame regions that delimit the detected lane.
 *  \param frame is a pointer to the image matrix of the current frame.
 *  \param lanes is the structure with the coefficients of the detected lane markers.
 *  \param thickness is the thickness in pixels of the drawn lines.
 */
    void FillLanes(cv::Mat* ,t_output_LD , unsigned int );
}


#endif
