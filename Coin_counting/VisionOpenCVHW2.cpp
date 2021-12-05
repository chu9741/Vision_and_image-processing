// VisionOpenCVHW2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;
void Find(int num);

int main()
{
    int num = 5;
    for (int i = 0; i <= num; i++) {
        Find(i);
        waitKey();
    }
    return 0;
}

void Find(int num) {
    String path[6] = {"C:/Users/USER/source/repos/VisionOpenCVHW2/Coins/coins0.jpg", 
        "C:/Users/USER/source/repos/VisionOpenCVHW2/Coins/coins1.jpg","C:/Users/USER/source/repos/VisionOpenCVHW2/Coins/coins2.jpg",
        "C:/Users/USER/source/repos/VisionOpenCVHW2/Coins/coins3.jpg","C:/Users/USER/source/repos/VisionOpenCVHW2/Coins/coins4.jpg",
        "C:/Users/USER/source/repos/VisionOpenCVHW2/Coins/coins5.jpg"};
    

    Mat image = cv::imread(path[num], 0);
    imshow("Original_image", image);
    Mat cannyimage;
    Mat cannyimage2;
    Mat GBlur;
    Mat GBlur2;
    Mat gray;
    //cvtColor(image,cannyimage2, COLOR_BGR2GRAY);
    GaussianBlur(image, GBlur2, Size(13, 13),0); // kernel size, sigma 
    Canny(GBlur2, cannyimage2, 50, 150, 3); // threshold low, high, kernel 
    imshow("detected circles", cannyimage2);
    waitKey();
    vector<Vec3f> circles;
    HoughCircles(cannyimage2, circles, HOUGH_GRADIENT, 1, cannyimage2.rows / 8, 100, 20, 30, 90); //

    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center

        circle(cannyimage2, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(cannyimage2, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
    }
    //imshow("detected circles", image);
    //waitKey();

    //imshow("Image", cannyimage);
    imshow("Counting_Coins", cannyimage2);
    printf("coin%d.jpg 의 circle개수 : %d\n",num, circles.size());
    
}

/*
1. Filter image with x, y derivatives of Gaussian 
2. Find magnitude and orientation of gradient
3. Non-maximum suppression:
Thin multi-pixel wide “ridges” down to single pixel width
4. Thresholding and linking (hysteresis):
Define two thresholds: low and high
Use the high threshold to start edge curves and the low threshold to continue them

MATLAB: edge(image, ‘canny’)
OpenCV: Canny( src, dst, lowThreshold, highTreshold, kernel_size )
                 It needs Gaussian, first
*/
