#include "ObjectDetection.h"

std::vector <cv::Ptr<cv::TrackerCSRT>> reset_list_of_trackers(std::vector<cv::Ptr<cv::TrackerCSRT>> trackers)
{
	if (!trackers.empty())
		trackers.clear();
	return trackers;
}
cv::Rect2d update_and_draw(std::vector<cv::Ptr<cv::TrackerCSRT>> trackers, cv::Mat frame, cv::Rect2d box)
{
	bool ok;
	std::list<cv::Ptr<cv::TrackerCSRT>>::iterator it;
	for (unsigned int i = 0; i < trackers.size(); ++i) {
		ok = trackers[i]->update(frame, box);
		//the code below draws a rectangle on a frame
		/*if (ok) {
			cv::rectangle(frame, box, Scalar(0, 0, 255), 2);
		}*/
	}
	return box;
}

std::list<cv::Rect2d>  ObjectDetection(cv::Mat frame,cv::CascadeClassifier classifier)
{
	cv::Mat frame,gray;
	static int frame_count = 0;
	std::vector<cv::Rect> cars;
	std::vector<cv::Ptr<cv::TrackerCSRT>> trackers;
	cv::Rect2d bbox;
	cv::Ptr<cv::TrackerCSRT> tracker;
	std::list<cv::Rect2d> bboxes;

	//the code below may be unnecessary
	/*if (!cap.isOpened()) {
		cout << "Could not open the video" << endl;
		exit(EXIT_FAILURE);
	}*/


		if (!bbox.empty()) {
			bboxes.push_back(update_and_draw(trackers, frame, bbox));
		}

		if (frame_count == 15) {
			trackers=reset_list_of_trackers(trackers);
			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
			cv::equalizeHist(gray, gray);
			classifier.detectMultiScale(gray, cars, IMG_SIZE_REDUCING_FACTOR, NEIGHBORS_TO_RETAIN);
			for (unsigned int i = 0; i < cars.size(); ++i) {
				bbox = cars[i];
				tracker = cv::TrackerCSRT::create();
				tracker->init(frame, bbox);
				trackers.push_back(tracker);
			}
			frame_count = 0;
		}
		return bboxes;

}
