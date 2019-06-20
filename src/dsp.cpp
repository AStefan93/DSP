/** \mainpage Digital Signal Processing
 *  \section Introduction
 *  This project is for learning purposes only.
 *  The main functions that will run inside the DSP project are LaneDetection(LD) and ObjectDetection(OD).
 *  The purpose is to integrate students in the ADAS high-level systems world.
 *
 *  \section a To be done
 *  TODO: Add architecture - Hardware + Software\n
 *  TODO: Detailed description of LD and OD.\n
 *  TODO: Comment DSP functions.\n
 *  TODO: Change interfaces.
 *
 *  Version: 1.0
 *  Written by Arnautu Stefan
 */

/** \file dsp.cpp
 *  \brief Digital Signal Processing Main Source File.
 *
 *  This file should include the main application, grabbing of frames and calling of vision components, like LD and OD.
 */

#include <dsp.h>

///Image resize factor to improve processing speed. This variable should be used in most vision components.
const double imgresize = 0.5;

#if USE_VIDEO

// the points for the video LCF_CameraView.avis
//src=source picture
const cv::Point2f src_vertices[4] = {cv::Point(530*imgresize,400*imgresize),cv::Point(649*imgresize, 400*imgresize),cv::Point(800*imgresize, 600*imgresize),cv::Point(300*imgresize, 600*imgresize)};

//dst=destination picture
const cv::Point2f dst_vertices[4] = {cv::Point(200*imgresize, 2*imgresize),cv::Point(550*imgresize, 2*imgresize),cv::Point(550*imgresize, 299*imgresize),cv::Point(200*imgresize, 299*imgresize)};

#else
//points for image sequence 00
//src=source picture
const cv::Point2f src_vertices[4] = {cv::Point(628*imgresize, 289*imgresize), cv::Point(736*imgresize, 289*imgresize), cv::Point(910*imgresize, 511*imgresize), cv::Point(343*imgresize, 511*imgresize)};

//dst=destination picture
const cv::Point2f dst_vertices[4] = {cv::Point(200*imgresize, 2*imgresize), cv::Point(550*imgresize, 2*imgresize), cv::Point(550*imgresize, 299*imgresize), cv::Point(200*imgresize, 299*imgresize)};

#endif // USE_VIDEO

t_generic_DSP generic_DSP;
t_output_LD output_LD;
t_output_KalmanLD output_KalmanLD;
const t_processModel_LD processModelLD;

///TODO: Object Detection Test Function. To be removed.
unsigned int ObjectDetection(cv::Mat image){

    cv::Mat image2(image.rows,image.cols,CV_8UC1);
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    return 1;

}

/** \brief Draw Lanes is an LD Drawing Function.
 *
 *  This function draws over only the inner and outer edges of the detected markers.
 *  \param [in] frame is a pointer to the image matrix of the current frame.
 *  \param [in] corridor is the array with coefficients of the corridor.
 *  \param [in] thickness is the thickness in pixels of the drawn lines.
 */
void DSP::DrawLanes(cv::Mat* frame, cv::Mat corridor, cv::Scalar color, unsigned int thickness){

    cv::Mat tempImage;
    tempImage = cv::Mat::zeros(400*imgresize,750*imgresize,CV_8UC3);//300+100 to fill an entire marker

    // we copy the 4 coefficients form dst to a,b,c,d
    float a,b,c,d;
    a=corridor.at<float>(3,0);
    b=corridor.at<float>(2,0);
    c=corridor.at<float>(1,0);
    d=corridor.at<float>(0,0);

    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        // we print that point
        // line creates a lane between 2 points, but if we give the same point, it will paint a point
        // scalar(x,y,x) is the color, 2=pixel width of eat printed point
        cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),color,thickness);
    }

    cv::warpPerspective(tempImage, tempImage, generic_DSP.M.inv(), (*frame).size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    (*frame) = (*frame) + tempImage;

}

/** \brief Fill Lanes is an LD Drawing Function.
 *
 *  This function draws over the original frame regions that delimit the detected lane.
 *  \param frame is a pointer to the image matrix of the current frame.
 *  \param lanes is the structure with the coefficients of the detected lane markers.
 *  \param thickness is the thickness in pixels of the drawn lines.
 */
void DSP::FillLanes(cv::Mat* frame, t_output_LD lanes, unsigned int thickness){

    cv::Mat tempImage;
    tempImage = cv::Mat::zeros(400*imgresize,750*imgresize,CV_8UC3);//300+100 to fill an entire marker

    float a,b,c,d;
    std::vector<cv::Point> points_outer_left;
    std::vector<cv::Point> points_inner_left;
    std::vector<cv::Point> points_inner_right;
    std::vector<cv::Point> points_outer_right;

    cv::Scalar purple = cv::Scalar(125,0,125);
    cv::Scalar blue = cv::Scalar(255,0,0);
    cv::Scalar green = cv::Scalar(0,125,0);


    //outer left lane
    a=lanes.OCL.at<float>(3,0);
    b=lanes.OCL.at<float>(2,0);
    c=lanes.OCL.at<float>(1,0);
    d=lanes.OCL.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_outer_left.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),purple,thickness);

    }
    //inner left lane
    a=lanes.ICL.at<float>(3,0);
    b=lanes.ICL.at<float>(2,0);
    c=lanes.ICL.at<float>(1,0);
    d=lanes.ICL.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_inner_left.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),purple,thickness);

    }
    //inner right lane
    a=lanes.ICR.at<float>(3,0);
    b=lanes.ICR.at<float>(2,0);
    c=lanes.ICR.at<float>(1,0);
    d=lanes.ICR.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_inner_right.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),blue,thickness);

    }
    //outer right lane
    a=lanes.OCR.at<float>(3,0);
    b=lanes.OCR.at<float>(2,0);
    c=lanes.OCR.at<float>(1,0);
    d=lanes.OCR.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_outer_right.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),blue,thickness);

    }

    for(int x=0;x<tempImage.rows-1;x++)
    {

        cv::line(tempImage,points_inner_left[x],points_outer_left[x],purple,thickness); //Draw left lane
        cv::line(tempImage,points_inner_right[x],points_outer_right[x],blue,thickness); //Draw right lane
        cv::line(tempImage,points_inner_left[x],points_inner_right[x],green,thickness); //Draw street in the middle

    }

    //warp the image back to real world view
    cv::warpPerspective(tempImage, tempImage, generic_DSP.M.inv(), (*frame).size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    (*frame) = (*frame) + tempImage;

}

/** \brief Fill Lanes is an LD Drawing Function.
 *
 *  This function draws over the original frame regions that delimit the detected lane.
 *  \param frame is a pointer to the image matrix of the current frame.
 *  \param lanes is the structure with the coefficients of the detected lane markers.
 *  \param thickness is the thickness in pixels of the drawn lines.
 */
void DSP::FillLanes(cv::Mat* frame, t_output_KalmanLD lanes, unsigned int thickness){

    cv::Mat tempImage;
    tempImage = cv::Mat::zeros(400*imgresize,750*imgresize,CV_8UC3);//300+100 to fill an entire marker

    float a,b,c,d;
    std::vector<cv::Point> points_outer_left;
    std::vector<cv::Point> points_inner_left;
    std::vector<cv::Point> points_inner_right;
    std::vector<cv::Point> points_outer_right;

    cv::Scalar purple = cv::Scalar(125,0,125);
    cv::Scalar blue = cv::Scalar(255,0,0);
    cv::Scalar green = cv::Scalar(0,125,0);


    //outer left lane
    a=lanes.priorOCL.at<float>(3,0);
    b=lanes.priorOCL.at<float>(2,0);
    c=lanes.priorOCL.at<float>(1,0);
    d=lanes.priorOCL.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_outer_left.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),purple,thickness);

    }
    //inner left lane
    a=lanes.priorICL.at<float>(3,0);
    b=lanes.priorICL.at<float>(2,0);
    c=lanes.priorICL.at<float>(1,0);
    d=lanes.priorICL.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_inner_left.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),purple,thickness);

    }
    //inner right lane
    a=lanes.priorICR.at<float>(3,0);
    b=lanes.priorICR.at<float>(2,0);
    c=lanes.priorICR.at<float>(1,0);
    d=lanes.priorICR.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_inner_right.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),blue,thickness);

    }
    //outer right lane
    a=lanes.priorOCR.at<float>(3,0);
    b=lanes.priorOCR.at<float>(2,0);
    c=lanes.priorOCR.at<float>(1,0);
    d=lanes.priorOCR.at<float>(0,0);
    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<tempImage.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        points_outer_right.push_back( cv::Point(y,x));
        //cv::line(tempImage,cv::Point(y,x),cv::Point(y,x),blue,thickness);

    }

    for(int x=0;x<tempImage.rows-1;x++)
    {

        cv::line(tempImage,points_inner_left[x],points_outer_left[x],purple,thickness); //Draw left lane
        cv::line(tempImage,points_inner_right[x],points_outer_right[x],blue,thickness); //Draw right lane
        cv::line(tempImage,points_inner_left[x],points_inner_right[x],green,thickness); //Draw street in the middle

    }

    //warp the image back to real world view
    cv::warpPerspective(tempImage, tempImage, generic_DSP.M.inv(), (*frame).size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    (*frame) = (*frame) + tempImage;

}

void *LaneDetection_Thread( void *ptr_image )
{
    generic_DSP.LD_Return_Value = LD::LaneDetection(*(cv::Mat *)ptr_image);
}

void *ObjectDetection_Thread( void *ptr_image )
{
    generic_DSP.OD_Return_Value = ObjectDetection(*(cv::Mat *)ptr_image);
}

/** \brief Main DSP Function.
 *
 *  This function is the main function called by the operating system.
 *  \param argc - is the iterator for the options given when the script is called
 *  \param argv - is a pointer to the options given when the script is called
 */

int main(int argc, char *argv[])
{

    clock_t begin = clock();

    generic_DSP.image_count = 0;
    generic_DSP.M = getPerspectiveTransform(src_vertices, dst_vertices); //Get birds eye view transform
    cv::Mat p_frame; 
    
    #if USE_VIDEO
    {//video input
        const char* videofilename = "../../../Work/Samples/LCF_CameraView.avi";

        cv::VideoCapture cap(videofilename); // open a video file
        if(!cap.isOpened())  // check if succeeded
        {
            std::cout << "file " << videofilename << " not found or could not be opened" << std::endl;
            return 0;
        }

        cap >> p_frame;//read first frame to  know the width and height

        //for recreating the video with the modified frames
        //cv::VideoWriter out_capture("../../road_out.avi", CV_FOURCC('M','J','P','G'), 25.0, cv::Size((p_frame).cols ,(p_frame).rows));

        // read frames until end of video:
        do{

            if((p_frame).empty()){
                std::cout << "frame is empty or could not be read" << std::endl;
                continue;
            }

            //resize image to improve performance
            cv::resize(p_frame,p_frame,cv::Size(),imgresize,imgresize);

            //3 separate threads for parallel processing of the same frame
            pthread_t thread1, thread2;
            int  iret1,iret2;

            iret1 = pthread_create( &thread1, NULL, LaneDetection_Thread, (void*) &p_frame);
            if(iret1)
            {
                fprintf(stderr,"error - pthread_create() return code: %d\n",iret1);
                exit(EXIT_FAILURE);
            }

            iret2 = pthread_create( &thread2, NULL, ObjectDetection_Thread, (void*) &p_frame);
            if(iret2)
            {
                fprintf(stderr,"error - pthread_create() return code: %d\n",iret2);
                exit(EXIT_FAILURE);
            }

            //wait for all threads to end
            pthread_join( thread1, NULL);
            pthread_join( thread2, NULL);

            //TODO: merge the info in env model


            //DRAW OBJECTS
            DSP::FillLanes(&p_frame, output_LD, 2);

            //save to video
            //out_capture.write((p_frame));

            //optional to show frame with detected objects/lanes/signs
            cv::namedWindow( "ld", CV_WINDOW_NORMAL );
            cv::imshow("ld", p_frame);
            cv::waitKey(1);
            generic_DSP.image_count += 1;
            std::cout << generic_DSP.image_count << std::endl;

        }while(cap.read(p_frame) && (generic_DSP.image_count < 3300));
    }
    //end video input
    #else
    {//data folder with individual frames input
        //const char* data_frames = "~/Work/Kitti/2011_09_26_drive_0015_extract/2011_09_26/2011_09_26_drive_0015_extract/image_00/data/%10d.png";
        const char* data_frames = "/home/stefan/Work/Kitti/2011_09_26_drive_0015_extract/2011_09_26/2011_09_26_drive_0015_extract/image_00/data/%10d.png";
        //aquire frames

        cv::VideoCapture sequence(data_frames); // open image sequence
        if(!sequence.isOpened())  // check if succeeded
        {
            std::cout << "file " << data_frames << " not found or could not be opened" << std::endl;
            return 0;
        }

        sequence >> p_frame;//read first frame to  know the width and height
        //write to video
        cv::VideoWriter out_capture("../road_out.avi", CV_FOURCC('M','J','P','G'), 25.0, cv::Size((p_frame).cols/2 ,(p_frame).rows/2), true);
        std::cout << p_frame.cols <<std::endl;
        std::cout << p_frame.rows <<std::endl;

        if(!out_capture.isOpened()){
            std::cout << "Video could not be opened for writing."<<std::endl;
            return 0;
        }

        // read frames until end of video:
        do{

            if((p_frame).empty()){
                std::cout << "frame is empty or could not be read" << std::endl;
                continue;
            }

            cv::resize(p_frame,p_frame,cv::Size(),imgresize,imgresize);

            //separate threads for parallel processing of the same frame
            pthread_t thread1, thread2;
            int  iret1,iret2;

            iret1 = pthread_create( &thread1, NULL, LaneDetection_Thread, (void*) &p_frame);
            if(iret1)
            {
                fprintf(stderr,"error - pthread_create() return code: %d\n",iret1);
                exit(EXIT_FAILURE);
            }

            iret2 = pthread_create( &thread2, NULL, ObjectDetection_Thread, (void*) &p_frame);
            if(iret2)
            {
                fprintf(stderr,"error - pthread_create() return code: %d\n",iret2);
                exit(EXIT_FAILURE);
            }

            //wait for all threads to end
            pthread_join( thread1, NULL);
            pthread_join( thread2, NULL);

            //TODO: merge the info in env model

            //draw lanes
            DSP::FillLanes(&p_frame, output_KalmanLD, 2);
//            DSP::FillLanes(&p_frame, output_LD, 2);

            //save to video
            out_capture.write((p_frame));

  //          cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
            //cv::imshow("Lane Detection", image_ld);
 //           cv::imshow("Display window", p_frame);
//            cv::waitKey(30);
            generic_DSP.image_count += 1;
            std::cout << generic_DSP.image_count << std::endl;

        }while(sequence.read(p_frame));
    }
    //end data folder with individual frames input
    #endif

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("%g\n",elapsed_secs);

    cv::waitKey(0);

    return 0;
}
