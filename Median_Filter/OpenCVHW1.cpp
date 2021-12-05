// OpenCVEX1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/imgproc.hpp>

using namespace cv;
//float k(int s, int t) { return exp(-( s*s + t*t)/100);  }
void median(Mat& src, int window, Mat& dst);
float k(int s, int t);
int main(int argc, const char* argv[])
{
    Mat f = imread("D:/MedianFilterInput.png", 0);
    f.convertTo(f, CV_32F, 1 / 255.f);
    Mat g(f.size(), CV_32FC1);
    
    median(f, 1, g);
    median(f, 2, g);
    median(f, 3, g);
    median(f, 4, g);
    median(f, 5, g);

    /*Mat kernel(10, 10, CV_32FC1);
    float wsum = 0;
    for (auto s = -10; s <= 10; s++) for (auto t = -10; t <= 10; t++) {
        kernel.at<float>(t + 10, s + 10) = k(s, t);
        wsum += k(s, t);
    }
    kernel /=wsum;


    for (auto y = 0; y < g.rows; y++) for (auto x = 0; x < g.cols; x++) { // replicate padding
        float sum = 0;
        for (auto s = -10; s <= 10; s++) for (auto t = -10; t <= 10; t++) {
            sum += kernel.at<float>(t+10, s+10) * f.at<float>(min(g.rows - 1, max(0, y + t)), min(g.cols - 1, max(0, x + s)));
        }
        g.at<float>(y, x) = sum;
    }

    ::imshow("this is image", g);
    ::waitKey();
    */
    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------

void median(Mat& src, int window, Mat& dst) {
    
    //std::vector<uchar> neighbors; neighbors.clear(); 
    //neighbots.push_back(val);
    //std::sort(neighbors.begin(), neighbors.end());
    //median 값은 neighbors[neighbors.size() / 2]
    float m = 0;
    for (auto y = 0; y < dst.rows; y++) for (auto x = 0; x < dst.cols; x++) { 
        std::vector<float>neighbors;
        for (auto s = -window; s <= window; s++) for (auto t = -window; t <= window; t++) {
            neighbors.push_back(src.at<float>(min(dst.rows - 1, max(0, y + t)), min(dst.cols - 1, max(0, x + s))));// replicate padding
        }
        std::sort(neighbors.begin(), neighbors.end());
        m = neighbors[neighbors.size() / 2];
        dst.at<float>(y, x) = m;
        neighbors.clear();
    }   
    
    //------------------------------------------
    imshow("this is image", dst);
    waitKey();
    
}
