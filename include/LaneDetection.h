/** \file LaneDetection.h
 *  \brief Lane Detection Header.
 *
 *  This file includes all the other resources the Lane Detection project requires.
 */

#ifndef LANEDETECTION_H
#define LANEDETECTION_H

#define LD_SHOW_GRAYSCALE (0)
#define LD_SHOW_BINARY (1)
#define LD_SHOW_BIRDSEYEVIEW (1)
#define LD_PRINT_SLIDINGWINDOWS (1)
#define LD_SHOW_SOBEL (1)
#define LD_SHOW_GAUSSFILTER (0)
#define LD_SHOW_ROIMASK (0)
#define LD_SHOW_ROI (0)

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include <dsp_typedef.h>

namespace LD{
    /** \brief Lane Detection Function.
     *
     *  This function is the Lane Detection function called by the higher DSP component.
     *  It calls all the other functions it requires for the full LD algorithm.
     *  \param OriginalImg - is the current frame passed to LD for processing.
     */
    unsigned int LaneDetection(cv::Mat OriginalImg);

    /**
    here we take a vector of points, and a destination matrix on wich we will
    save the coefficients of the polynom found by polyfit

    */
    void Polyfit(std::vector<cv::Point> VectorPoints, cv::Mat &dst);

    void polyfit_one(const cv::Mat& src_x, const cv::Mat& src_y, cv::Mat& dst, int order);

    /**
    This function will recieve two vector of points that will store the detected points of the lanes(left and right)
    using sliding window on the img(the third parameter of this function), wich will be a sobel image. The forth parameter is just am image on wich we draw the windows, to see them(is not essential)
    We will use the existing function Hitogram to determin the procent of white pixels in each SlidingWindow, and
    the peak(or peaks)
    */
    void SlidingWindow(std::vector<cv::Point> &left, std::vector<cv::Point> &right, cv::Mat img, cv::Mat &img3);

    /**
    Region of interest
    we take img1 and img2 , crop the region  we need from them
    and discard the rest(make black the parts that we dont need)
    */
    void ROI(cv::Mat &img1, cv::Mat &img2);

     /** \brief Horizontal_Sobel detects the vertical edges in an image.
     *
     *  This function applies a horizontal kernel based on the sobel edge detector.
     *  The input is a grayscale image and the output is two images containing LOW-HIGH edges and HIGH-LOW edges.
     *  \param [in] img is the input grayscale image.
     *  \param [out] img1 is the output sobel image with LOW-HIGH horizontal edges.
     *  \param [out] img2 is the output sobel image with HIGH-LOW horizontal edges.
     */
    void Horizontal_Sobel(cv::Mat img, cv::Mat &img1, cv::Mat &img2); //Exercise 3 SOBEL EDGE DETECTOR

    /**
    The function recieves OriginalImg (wich will be modified), img1, img2=the 2 sobel images(left and right)
    and the 4 arrays of points , wich will be also filled with the points detected for each
    of the 2 lanes(inner and outer)
    */
    void FindPointsLane(cv::Mat &OriginalImg, cv::Mat img1, cv::Mat img2, std::vector<cv::Point> &OuterLeft, std::vector<cv::Point> &InnerLeft, std::vector<cv::Point> &InnerRight, std::vector<cv::Point> &OuterRight);

    /**
    Transforms a Picture from Normal view To Bird's eye view
    (view of the road fron the top). We choose 4 points in the original picture
    that we know should be on a straight line in reality, then we choose 4 points
    in the New picture, wich will correspond to the 4 that we chose on the first picture
    */
    cv::Mat BirdsEyeView(cv::Mat img);

    /**
    we will receive a sobel image(right or left sobel)
    and for every column, we count the white pixels and we create a histogram
    but we will analyze only the lower half of the image (the second parameter of the function is percentage of image
                                                          ex 30=>30% of bottom image)
    because we will see the peak point of the 2 halfs of the histogram
    those peaks will be the beginning of the 2 lanes(left and right)
    we will need to know where the 2 lines begin, to later apply a method for detecting the lanes

    the thrid parameter of the function is the number of peaks we are looking for

    */
    int* Lane_Histogram(cv::Mat img, int procent, int peaks);


    /** \brief rgb2gray transforms an RGB color image to grayscale.
     *
     *  This function takes a color image and computes for each color pixel the grayscale equivalent.
     *  \param [in] img1 is the input color image.
     *  \param [out] img2 is the output grayscale image.
     */
     void rgb2gray(cv::Mat img1, cv::Mat img2); //Exercise 1 RGB2GRAY

     /** \brief meanFilter applies a 3x3 mean filter on the given image.
     *
     *  This function filters a given grayscale image with a 3x3 mean filter.
     *  \param [in] img1 is the input grayscale image.
     *  \param [out] img2 is the output grayscale filtered image.
     */
     void meanFilter(cv::Mat img1, cv::Mat img2); //Exercise 2 MEANFILTER
}

#endif //


