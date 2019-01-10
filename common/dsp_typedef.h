/** \file dsp_typedef.h
 *  \brief Digital Signal Processing Typedef Header.
 *
 *  This file defines all the common interfaces and should be included in all the components.
 */
#ifndef DSP_TYPEDEF_H
#define DSP_TYPEDEF_H

//Default OpenCV headers
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

extern const double imgresize; //!< Image resize factor - ranges from 0 to 1.
extern const cv::Point2f src_vertices[4]; //!< Source image vertices for finding the BEV matrix transform.
extern const cv::Point2f dst_vertices[4]; //!< Distance image vertices for finding the BEV matrix transform.

typedef struct t_generic_DSP{

    cv::Mat M; //!< Birds eye view matrix transform.
    unsigned int image_count; //!< image count of the current frame
    unsigned int LD_Return_Value; //!< return value of LD
    unsigned int OD_Return_Value; //!< return value of OD

}t_generic_DSP;
extern t_generic_DSP generic_DSP;

typedef struct t_input_LD{

    cv::Mat* frame;

}t_input_LD;
extern t_input_LD input_LD;

typedef struct t_output_LD{

    cv::Mat OCL,ICR; //!Coefficients Left, Right
    cv::Mat ICL,OCR; //!Coefficients Inner Left, Outer Right

}t_output_lD;
extern t_output_LD output_LD;

typedef struct t_input_OD{

    cv::CascadeClassifier classifier;
    unsigned int frame_count;
    cv::Mat* frame;

}t_input_OD;
extern t_input_OD input_OD;

typedef struct t_output_OD{

    //std::list<cv::Rect2d> bboxes;

}t_output_OD;
extern t_output_OD output_OD;

#endif // DSP_TYPEDEF_H
