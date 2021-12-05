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
    
    // Mat kernel(10, 10, CV_32FC1);
    /*float wsum = 0;
    for (auto s = -10; s <= 10; s++) for (auto t = -10; t <= 10; t++) {
        kernel.at<float>(t + 10, s + 10) = k(s, t);
        wsum += k(s, t); //m
    }
    kernel /= wsum;   */ // 중앙값을 kenrnel에넣고 /441
    //------------------------------------------
  
    /*
    for (auto t = -10; t <= 10; t++) for (auto s = -10; s <= 10; s++) {      
        kernel.at<float>(t + 10, s + 10) = k(s,t);      
    }
    */
   
   
    /*for (int i = -10; i < 10; i++) {
        neighbors[i+10] /= m;
    }*/

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

// Hint.
/*
픽셀 값들을 모아서 median을 계산하는 과정에 std::vector를 사용하면 편리합니다.
std::vector<uchar> neighbors; 와 같이 선언하고, neighbors.clear(); 로 기존에 쌓여 있던 data를 삭제합니다.
src의 해당 영역 내의 각 픽셀의 값을 추가 할 때는 neighbots.push_back(val); 과 같이 추가할 수 있습니다.
영역 내의 필셀이 모은 후, 정렬은 std::sort(neighbors.begin(), neighbors.end()); 와 같이 진행할 수 있습니다.
median 값은 neighbors[neighbors.size() / 2] 입니다
*/

/*Median Filter를 적용하기 위한 별도의 함수를 작성합니다. 함수는 입력 영상, median filter의 neighbor 크기, 출력 영상을 배개변수로 받습니다.
(예를 들면 void median( const cv::Mat& src, int window, cv::Mat& dst )와 같은 형식 입니다.)

2. 함수 내부에서 dst의 모든 픽셀을 순회하며,

   a. 각 필셀마다 src의 대상 영역 필셀 값들을 추출합니다.
   b. 이때 대상 영역은 x축으로 [-window, window] 이며, y축으로도 [-window, window]입니다.
   c. Padding은 replicate padding을 사용합니다.
   d. 추출된 픽셀 값을 정렬하여 중간 값을 찾고, 중간 값을 dst의 해당 픽셀에 저장합니다.

3. main 함수에서는

   a. 첨부된 MedianFilterInput.png 파일을 OpenCV를 이용하여 회색조(gray scale)로 읽어옵니다.
   b. 앞서 작성한 함수를 사용하여 window=1 일 때 median filter를 적용하고,
   c. imshow를 통하여 화면에 출력한 후 waitkey() 합니다.
   d. b-c 과정을 반복하여 window크기를 2, 3, 4, 5까지 바꾸어 가며 결과를 출력합니다.*/