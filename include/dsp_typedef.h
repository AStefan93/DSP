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

    cv::Mat OCL = cv::Mat::zeros(4,1, CV_32FC1);
    cv::Mat ICR = cv::Mat::zeros(4,1, CV_32FC1); //!Coefficients Left, Right
    cv::Mat ICL = cv::Mat::zeros(4,1, CV_32FC1);
    cv::Mat OCR = cv::Mat::zeros(4,1, CV_32FC1); //!Coefficients Inner Left, Outer Right

}t_output_LD;
extern t_output_LD output_LD;

typedef struct t_output_KalmanLD{

    cv::Mat priorOCL = cv::Mat::zeros(4, 1, CV_32FC1);
    cv::Mat priorICR = cv::Mat::zeros(4, 1, CV_32FC1); //!Predicted coefficients Left, Right
    cv::Mat priorICL = cv::Mat::zeros(4, 1, CV_32FC1);
    cv::Mat priorOCR = cv::Mat::zeros(4, 1, CV_32FC1); //!Predicted coefficients Inner Left, Outer Right
    cv::Mat posteriorOCL = cv::Mat::zeros(4, 1, CV_32FC1);
    cv::Mat posteriorICR = cv::Mat::zeros(4, 1, CV_32FC1); //!Estimated coefficients Left, Right
    cv::Mat posteriorICL = cv::Mat::zeros(4, 1, CV_32FC1);
    cv::Mat posteriorOCR = cv::Mat::zeros(4, 1, CV_32FC1); //!Estimated coefficients Inner Left, Outer Right

    cv::Mat prior_OCL_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);  
    cv::Mat prior_ICR_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);  
    cv::Mat prior_ICL_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);
    cv::Mat prior_OCR_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);
    cv::Mat posterior_OCL_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);  
    cv::Mat posterior_ICR_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);
    cv::Mat posterior_ICL_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1); 
    cv::Mat posterior_OCR_covariance = cv::Mat::zeros(cv::Size(4, 4), CV_32FC1);

}t_output_KalmanLD;
extern t_output_KalmanLD output_KalmanLD;

typedef struct t_processModel_LD{

    float data_R[16] = {1600,0,0,0, 0,10.0,0,0, 0,0,4.0,0, 0,0,0,0.4};
    float data_Q[16] = {16,0,0,0, 0,0.001,0,0, 0,0,0.004,0, 0,0,0,0.0004};
    const cv::Mat R = cv::Mat(4,4, CV_32FC1, data_R);
    const cv::Mat Q = cv::Mat(4,4, CV_32FC1, data_Q);

}t_processModel_LD;
const extern t_processModel_LD processModelLD;

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
