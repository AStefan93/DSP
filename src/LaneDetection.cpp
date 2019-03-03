/** \file LaneDetection.cpp
 *  \brief Lane Detection Main Source File.
 *
 *  This file should include the main application of the Lane Detection project.
 */

#include "../include/LaneDetection.h"

///MIDDLE OF TRAVEL LANE (Proportion)
#define middle(OriginalImg)  OriginalImg.cols/100*50
///<Needed for separating Left lanes from right lanes

///Minimul width in pixels of a line to be considered lane
#define LaneWidth(OriginalImg)   OriginalImg.cols/100*5
/**<
Used in the algorithm for determining if the space between
right Sobel and left Sobel is smaller than LaneWidth
*/


#define SlidingWindowWidth(img)     img.cols*10/100
/**
used for setting the width of the sliding window as a percentage of the image width
*/

#define StartingPointsPercentage    30
/**
used when first calling SlidingWindow , to specify in wich percentage of the image we will search the 2 starting points(from the bottom of the image)
*/


/**
max deviation in percentage. ex 30 means if the deviation between 2 sliding windows is greater than 30% of SlidingWindowWidth, we ignore that deviation. This helps to ignore false detections
*/


void LD::rgb2gray(cv::Mat img1, cv::Mat img2){

   //write code here
}

void LD::meanFilter(cv::Mat img1, cv::Mat img2){

   //write code here
}

cv::Mat LD::BirdsEyeView(cv::Mat img)
{
    // we create the dst picture that we will return(rows, cols, type)
    cv::Mat dst(300*imgresize, 750*imgresize, CV_8UC3);
    cv::warpPerspective(img, dst, generic_DSP.M, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    #if ((LD_SHOW_BIRDSEYEVIEW) && !(LD_PRINT_SLIDINGWINDOWS))
        cv::imshow("BirdsEyeView",dst);
    #endif // LD_SHOW_BIRDSEYEVIEW

    return dst;
}

void LD::Horizontal_Sobel(cv::Mat img, cv::Mat &img1, cv::Mat &img2)
{
    int sobel;

    for(unsigned int i = 1; i < img.rows - 1; i++){
        for(unsigned int j = 1; j < img.cols - 1; j++){
            sobel = img.at<uchar>(i-1,j-1)*-1 + img.at<uchar>(i-1,j  )*0 + img.at<uchar>(i-1,j+1)*1+
                    img.at<uchar>(i  ,j-1)*-2 + img.at<uchar>(i  ,j  )*0 + img.at<uchar>(i  ,j+1)*2+
                    img.at<uchar>(i+1,j-1)*-1 + img.at<uchar>(i+1,j  )*0 + img.at<uchar>(i+1,j+1)*1;

            if(sobel < 0){
                img2.at<uchar>(i,j) =-sobel/8;
            }else{
                img1.at<uchar>(i,j) = sobel/8;
            }
        }
    }

}

void LD::ROI(cv::Mat &img1, cv::Mat &img2)
{
    //array of points
    std::vector<cv::Point> points;

    //The points ORDER is important
    //we push back the points that delimitates the region wich interest us
    points.push_back( cv::Point(100*imgresize,2*imgresize)); //0,0
    points.push_back( cv::Point(650*imgresize,2*imgresize)); // 750,0
    points.push_back( cv::Point(650*imgresize,299*imgresize)); //750, 300
    points.push_back( cv::Point(100*imgresize,299*imgresize)); //104, 300

    //make an empty mask the dimensions of img1, CV_8UC1=8bit pixel with 1 channel(white and black)
    cv::Mat mask1 = cv::Mat::zeros( img1.size(), CV_8UC1 );


    // Initialize the polygon delimited by the points with white
    fillConvexPoly( mask1, points, cv::Scalar( 255 ));
    cv::Mat mask2=mask1.clone();

    #if LD_SHOW_ROIMASK
        cv::imshow("MASK", mask1);
    #endif // LD_SHOW_ROIMASK


    // create a new image by copying only the rectangle from the img
    cv::Mat roi1 = img1.clone();

    //WHAT WE PASTE

    cv::Mat roi2 = img2.clone();

    // form is now the part of mask1 that contains the ROI delimited by the points
    cv::Mat form = mask1.clone();
    // HOW WE PASTE(In wich form)

     // imgROI is the adress at wich we will paste the ROI
    cv::Mat imgROI1 = mask1;
    cv::Mat imgROI2 = mask2;
    // WHERE WE PASTE

    // copy roi(the part that we need from the entire photo) to: imgROI(the adrees=the place),form=the form in wich we want it to pe pasted
    roi1.copyTo(imgROI1, form);
    roi2.copyTo(imgROI2, form);

    //now we copy the result in img1 and img2, wich will remain modified
    img1=mask1.clone();
    img2=mask2.clone();

}

void LD::FindPointsLane(cv::Mat &OriginalImg, cv::Mat img1, cv::Mat img2, std::vector<cv::Point> &OuterLeft, std::vector<cv::Point> &InnerLeft, std::vector<cv::Point> &InnerRight, std::vector<cv::Point> &OuterRight)
{
    int k=0;

    for(int i=img1.rows-3;i>3;i--)
    {

        for(int j=3;j<img1.cols-3;j++)
        {

            // if we are on the middle column, we change that pixel to red
            //(to print the middle line, that separes the 2 lanes)
            if(j==middle(OriginalImg))
            {
                OriginalImg.at<cv::Vec3b>(i,j)[0] = 0;
                OriginalImg.at<cv::Vec3b>(i,j)[1] = 0;
                OriginalImg.at<cv::Vec3b>(i,j)[2] = 255;
            }

            /*
            k counts the points found at each row between the white on the left sobel and
            the white in the right sobel. if k becomes greater than the LaneWidth
            that means that this is not a lane, so we dont need to save those points
            and k becomes 0 again
            */
            if(k > LaneWidth(OriginalImg))
                k=0;

            /*
            if k>0 that means that we found a white pixel in the left sobel image
            so we need to continue to search from that point, but in the left sobel
            to find the end on the Lane. While we search , k increase
            and when it become greater than LaneWidth, that means this is not a
            Lane portion
            */
            if(k>0 )
            {
                // we check if we found the corresponding point in the right sobel img
                if(img2.at<uchar>(i,j) ==255)
                {
                    // if j< middle, that means we are on the left side of the image
                    if(j<middle(OriginalImg))
                    {
                        // so we push the point in the Inner Left vector, but also
                        //the point -k wich is the beginning of the lane (the left side of the lane)
                        InnerLeft.push_back( cv::Point(i,j));
                        OuterLeft.push_back( cv::Point(i,j-k));

                    }
                    else
                    {

                        InnerRight.push_back( cv::Point(i,j-k));
                        OuterRight.push_back( cv::Point(i,j));

                    }


                }
                else
                    k++;
                    // if we didnt find yet the corresponding point in the right sobel img
                    // we keep searchiing, while increasing k
            }
            else    if(img1.at<uchar>(i,j) ==255)
                    {
                        // here, if we found 1 white pixel in the left sobel img then
                        // we make k=1, to start searching in the right sobel img
                        k=1;
                    }

        }

    }
}

void LD::polyfit_one(const cv::Mat& src_x, const cv::Mat& src_y, cv::Mat& dst, int order)
{
    CV_Assert((src_x.rows>0)&&(src_y.rows>0)&&(src_x.cols==1)&&(src_y.cols==1)
            &&(dst.cols==1)&&(dst.rows==(order+1))&&(order>=1));
    cv::Mat X;
    X = cv::Mat::zeros(src_x.rows, order+1,CV_32FC1);
    cv::Mat copy;
    for(int i = 0; i <=order;i++)
    {
        copy = src_x.clone();
        pow(copy,i,copy);
        cv::Mat M1 = X.col(i);
        copy.col(0).copyTo(M1);
    }
    cv::Mat X_t, X_inv;
    transpose(X,X_t);
    cv::Mat temp = X_t*X;
    cv::Mat temp2;
    invert (temp,temp2);
    cv::Mat temp3 = temp2*X_t;
    cv::Mat W = temp3*src_y;
    W.copyTo(dst);
}

void LD::Polyfit(std::vector<cv::Point> VectorPoints, cv::Mat &dst)
{
    // we create 2 Mats(matrix), with 1 column and z rows
    // where z=the number of elements of the VectorPoints
    cv::Mat src_x = cv::Mat(VectorPoints.size(), 1, CV_32F);
    cv::Mat src_y = cv::Mat(VectorPoints.size(), 1, CV_32F);
    //dst will be a matrix of 1 column and grade+1
    // where grade=grade of the polynom
    dst = cv::Mat(4, 1, CV_32F);


    // for each element of the array, we save its x on the src_x matrix
    // and its y on the src_y matrix
    for (int i = 0; i < VectorPoints.size(); i++)
    {
        src_y.at<float>(i, 0) = VectorPoints[i].y;     //adica y in imagine
        src_x.at<float>(i, 0) = VectorPoints[i].x;     //adica x in imagine
    }

    // we apply polyfit, wich will result in 4 coefficients saved in dst
    LD::polyfit_one(src_x, src_y,dst, 3);
}

void PrintLines(cv::Mat &OriginalImg, cv::Mat dst,int x,int y,int z)
{
    // we copy the 4 coefficients form dst to a,b,c,d
    float a,b,c,d;
    a=dst.at<float>(3,0);
    b=dst.at<float>(2,0);
    c=dst.at<float>(1,0);
    d=dst.at<float>(0,0);


    // for each row of the Original img(wich will be the x)
    // we will calculate the y by using the 4 coefficients of the polynom
    for(int x=0;x<OriginalImg.rows-1;x++)
    {
        float y=a*pow(x,3)+b*pow(x,2)+c*x+d;
        // we print that point
        // line creates a lane between 2 points, but if we give the same point, it will paint a point
        // scalar(x,y,x) is the color, 2=pixel width of eat printed point
        cv::line(OriginalImg,cv::Point(y,x),cv::Point(y,x),cv::Scalar(x,y,z),5);
    }
}

int* LD::Lane_Histogram(cv::Mat img, int procent, int peaks)
{

    int maxSum1=0,maxSum2=0;
    int xPeak1=0, xPeak2=0;
    // result[0]   will be the numbers of (white points)/ (all pixels)     (between 0 and 100)
    // result[1]   wil lbe the first peak, and (optionally) result[2] the second one
    int* result=new int[peaks+1];

    //totalSum will be the total number of white pixels in the image
    int totalSum=0;

    for(int i=4;i<img.cols-4;i++)
    {
        // for each column, the sum resets
        // sum is the number of white pixels found on each column
        int sum=0;

        //in the current column, we start from the bottom till the (100-procent) of the image (ex procent=30 , so we go from 100% to 70% of image, starting from the bottom)
        for(int j=img.rows;j>img.rows*(100-procent)/100 ;j--)
        {

            //and if the pixel is white
            if((j > 0) && (j < img.rows) && (i > 0) && (i < img.cols)){
                if(255==img.at<uchar>(j,i)) //j=line, i=column
                {
                    // we increment sum
                    sum++;
                    totalSum++;
                }
            }
        }

        // find the peaks of the histogram

        if(peaks==1)
        {
            //if we find a column with sum of white pixels greater than max, then we save that column (the i column), and its number of white pixels in maxSum
            if(sum>maxSum1)
            {
                maxSum1=sum;
                xPeak1=i;
            }

        }
        else if(peaks==2)
        {
            int middleOfImage=img.cols/2;
            if(i<middleOfImage)         //we search the left pick
            {
                if(sum>maxSum1)
                {
                    maxSum1=sum;
                    xPeak1=i;
                }
            }
            if(i>=middleOfImage)         //we search the right pick
            {
                if(sum>maxSum2)
                {
                    maxSum2=sum;
                    xPeak2=i;
                }
            }

        }

    }
    //peak==1 we put peak1 in the result pointer, peak==2, we put both peaks
    peaks==1?(result[1]=xPeak1):(result[1]=xPeak1,result[2]=xPeak2);
    result[0]=totalSum/(img.rows*img.cols);
    return result;
}

void LD::SlidingWindow(std::vector<cv::Point> &left, std::vector<cv::Point> &right, cv::Mat img, cv::Mat &img3)
{
    // we will first apply SlidingWindow on the img, so we see the 2 starting points of the lanes.Those will be the middles of the 2 windows

    // this pointer will be a vector with 1 or 2 parameters returned by the function Histogram
    int *pointer;

    //we apply the histogram to 30% of the bottom of img, and expect 2 points from it  (StartingPointsPercentage is a #define with the value 30)!!
    pointer=LD::Lane_Histogram(img,StartingPointsPercentage,2);

    // a and b will be the 2 starting points(columns)
    int a=pointer[1];
    int b=pointer[2];

    //first points that marks the beginning of the first sliding windows. We put them in the arrays
    left.push_back (cv::Point(img.rows, a));
    right.push_back (cv::Point(img.rows, b));

    // sliding window will be 5% of img.cols, SlidingWindowWidth (another  #define)
    //we use only the 2 sliding windows from this row, and call Histogram for them, to see where the middle will be,then we add them to the vectors
    for(int i=img.rows*90/100;i>=0;i-=img.rows/20)  //img.rows/20 means %5 of the number or rows
    {
        //parameters for the rectangle
        int x,y,width,height;

        //starting X point of the rectangle. We test if the rectangle is out of the img, then we put t to begin at pixel 1, because a rectangle cant exist outside of one image
        x=a-SlidingWindowWidth(img)/2>0 ?   a-SlidingWindowWidth(img)/2 :   1;
        //Starting Y point of the rectangle
        y=i;
        //width of the rectangle
            width=SlidingWindowWidth(img);
        //height of the rectangle
            height=img.rows/20;

        // we create the left rectangle: r1
        cv::Rect r1(x,y,width,height);

        // The parameters for the right rectangle
        x=b-SlidingWindowWidth(img)/2 <  img.cols-SlidingWindowWidth(img) ?   b-SlidingWindowWidth(img)/2   :   img.cols-SlidingWindowWidth(img)-1;
        y=i;
        width=SlidingWindowWidth(img);
        height=img.rows/20;

        //we create the right rectangle
        cv::Rect r2(x,y,width,height);

        // we call Histogram function for the left window.  img(r1) means we call the function giving only the rectangle part of img
        pointer=LD::Lane_Histogram(img(r1),90,1);

         //if the peak of the window is on one of the extremities , that means the window does not contain white, so we do not update the "a" variable
        if(pointer[1]>5 && pointer[1] < SlidingWindowWidth(img)-5 )
        {
            a=pointer[1]+a-SlidingWindowWidth(img)/2;

        }

        // we call Histogram function for the right window.
        pointer=LD::Lane_Histogram(img(r2),90,1);

        if(pointer[1]>5 && pointer[1] < SlidingWindowWidth(img)-5 )  //if the peak of the window is on one of the extremities , that means the window does not contain white, so we do not update the "a" variable
        {
            b=pointer[1]+b-SlidingWindowWidth(img)/2;

        }

        //  we put the points in the vectors.      The point is X= a or b,    and Y=Between the rows. (If the for gives "i" values that are every 5% of the rows, we give it another 2.5% that is img.rows/40, so it will be 5%+2.5%=7.5%.....)See the paper in the Drawer, I draw this
        left.push_back (cv::Point(i+img.rows/40, a));
        right.push_back (cv::Point(i+img.rows/40, b));

        //We draw the rectangles(sliding windows) on another image, ant then we will imgshow it in Main
        #if LD_PRINT_SLIDINGWINDOWS
            rectangle(img3,r1,cv::Scalar(255,0,0),1,4,0);
            rectangle(img3,r2,cv::Scalar(255,0,0),1,4,0);
        #endif

    }

    #if (LD_PRINT_SLIDINGWINDOWS && LD_SHOW_BIRDSEYEVIEW)
        cv::imshow("Sliding Windows",img3);
    #endif

}

unsigned int LD::LaneDetection(cv::Mat OriginalImg)
{
	//Check if was opened correctly
	if(OriginalImg.empty())
	{

        return 0;
	}

    //RGB to Gray convertion
    cv::Mat grayImage(OriginalImg.rows,OriginalImg.cols,CV_8UC1);
    cv::cvtColor(OriginalImg, grayImage, CV_BGR2GRAY);
    //Exercise 1 RGB2GRAY
    //LD::rgb2gray(OriginalImg, grayImage);

    #if LD_SHOW_GRAYSCALE
        cv::imshow("Grayscale",grayImage);
    #endif

    //Bird's Eye View
    cv::Mat img = LD::BirdsEyeView(grayImage);

	//Eliminate noise
	cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0);
	//Exercise 2 MEANFILTER
	//LD::meanFilter(img, img);


	#if LD_SHOW_GAUSSFILTER
        cv::imshow("Gaussian Filter",img);
	#endif // LD_SHOW_GAUSSFILTER


    //Apply Sobel
    cv::Mat img1;//sobel left
    cv::Mat img2;//sobel right
    img1 = cv::Mat::zeros(img.rows,img.cols,CV_8UC1);
    img2 = cv::Mat::zeros(img.rows,img.cols,CV_8UC1);

    //TEST SOBEL EDGE DETECTOR
    //int dist_x = 0; //try switching x and y to detect horizontal edges instead of vertical edges
    //int dist_y = 1;
    //int window_size = 3; //should be 3,5 or 7
    //cv::Mat img_sobel_test;
    //img_sobel_test = cv::Mat::zeros(img.rows,img.cols,CV_8UC1);
    //cv::Sobel( img, img_sobel_test, CV_8UC1, dist_x, dist_y, window_size, 1, 0, cv::BORDER_DEFAULT );
    //cv::convertScaleAbs( img_sobel_test, img_sobel_test );
    //cv::imshow("sobel test",img_sobel_test);

    //cv::Sobel( img, img1, CV_8UC1, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT );
    //cv::Sobel( img, img2, CV_8UC1, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT );
    LD::Horizontal_Sobel(img,img1,img2); //EXERCISE 3 SOBEL FILTER

    #if LD_SHOW_SOBEL
        cv::imshow("Horizontal Sobel LEFT",img1);
        cv::imshow("Horizontal Sobel RIGHT",img2);
    #endif // LD_SHOW_SOBEL

    //BINARIZARE
    cv::threshold(img1, img1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    cv::threshold(img2, img2, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    #if LD_SHOW_BINARY
        cv::imshow("Binary image LEFT",img1);
        cv::imshow("Binary image RIGHT",img1);
    #endif // LD_SHOW_BINARY


    //ROI
    LD::ROI(img1,img2);

    #if LD_SHOW_ROI
        cv::imshow("ROI",img1);
    #endif // LD_SHOW_ROI

    //FIND POINTS WITH SLIDING WINDOW
    std::vector<cv::Point> OuterLeftLine;
    std::vector<cv::Point> InnerRightLine;
    std::vector<cv::Point> InnerLeftLine;
    std::vector<cv::Point> OuterRightLine;
    LD::SlidingWindow(OuterLeftLine,InnerRightLine,img1,img);
    LD::SlidingWindow(InnerLeftLine,OuterRightLine,img2,img);

    //TODO: Transform points to 3D world coordinates

    //Polynomial Regression
    LD::Polyfit(OuterLeftLine, output_LD.OCL);
    LD::Polyfit(InnerRightLine, output_LD.ICR);
    LD::Polyfit(InnerLeftLine, output_LD.ICL);
    LD::Polyfit(OuterRightLine, output_LD.OCR);

    //TODO: Apply Kalman Filter

    return 1;
}























