#pragma once
#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/ocl.hpp>
//#include <opencv2/tracking/tldDataset.hpp>
//#include <opencv2/tracking/tracking.hpp>
#include <iostream>
#include <tuple>
#include <list>
#include<vector>
#include<string>

#define IMG_SIZE_REDUCING_FACTOR 1.3 //scaleFactor = Parameter specifying how much the image size is reduced at each image scale.
#define NEIGHBORS_TO_RETAIN 20//minNeighbors = Parameter specifying how many neighbors each candidate rectangle should have to retain it.

std::vector <cv::Ptr<cv::TrackerCSRT>> reset_list_of_trackers(std::vector<cv::Ptr<cv::TrackerCSRT>> trackers);
cv::Rect2d update_and_draw(std::vector<cv::Ptr<cv::TrackerCSRT>> trackers, cv::Mat frame, cv::Rect2d box);
std::list<cv::Rect2d> ObjectDetection(cv::Mat frame, cv::CascadeClassifier classifier);
