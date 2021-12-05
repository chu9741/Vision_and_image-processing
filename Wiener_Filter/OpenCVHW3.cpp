// OpenCVHW3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
int main(int argc, const char* argv[] )
{
    float K = 0.002;
    Mat g = imread("D:/이전학기/3-2/OpenCVHWs/OpenCVHW3/Wiener_Input2.png", 0); //// blurred image , g
    //D:\이전학기\3-2\OpenCVHWs\OpenCVHW3
    g.convertTo(g, CV_32FC2, 1 / 255.f);
    Mat G;
    dft(g, G, DFT_COMPLEX_OUTPUT);
    imshow("Original_image", g);
    Mat h = imread("D:/이전학기/3-2/OpenCVHWs/OpenCVHW3/Wiener_Kernel.png",0);   /// kernel , h, psf
    h.convertTo(h, CV_32FC2, 1 / 255.f);
    h /= sum(h);
    Mat H;
    dft(h, H, DFT_COMPLEX_OUTPUT);

    //imshow("G");
    // imshow("H", h);

    Mat G2;
    mulSpectrums(G, H, G2, 0, true); //분자 계산,  G = G * H*
    Mat H2;
    std::vector<Mat> channels;
    split(H, channels); 
    magnitude(channels[0], channels[1], H2); // H2 = |H|  sqrt(aa+bb)  분모부분
    
    //mulSpectrums(H2, H2, H2, 0); // H2의 제곱
    multiply(H2,H2,H2);
    
    std::vector<Mat> channels2; 
    split(G2, channels2); //분자를 channel을 나눔
    
    divide(channels2[0],H2+K, channels2[0]);  //c/R
    divide(channels2[1],H2+K, channels2[1]);  // d/Ri
    merge(channels2 ,G2); //(c+di)/R = c/R + d/Ri 이므로 두 channel 모두 D가 되어 있는 두 channel image  G * H* / H2
    
 
    //Mat F= Mat::zeros(G.size(), CV_32FC2);
    //divide(G2, H2, F);
    idft(G2, g, DFT_SCALE | DFT_REAL_OUTPUT);
    imshow("Wiener Filter",g);
    waitKey();
    return 0;
    
}

