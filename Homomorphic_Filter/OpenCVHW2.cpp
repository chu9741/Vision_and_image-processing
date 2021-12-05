// OpenCVHW2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace cv;

void showFFT(const std::string& name, const Mat& f) {
    int w = f.cols / 2;
    int h = f.rows / 2;
    Mat M;
    std::vector<Mat> channels;
    split(f, channels);
    magnitude(channels[0], channels[1], M);

    Mat c(f.size(), CV_32FC1);
    M(Rect(0, 0, w, h)).copyTo(c(Rect(w, h, w, h)));
    M(Rect(w, 0, w, h)).copyTo(c(Rect(0, h, w, h)));
    M(Rect(0, h, w, h)).copyTo(c(Rect(w, 0, w, h)));
    M(Rect(w, h, w, h)).copyTo(c(Rect(0, 0, w, h)));
    log(c + 1, c);
    normalize(c, c, 0, 1, NORM_MINMAX);
    imshow(name, c);
}

void homofilter(float gH, float gL, float D0, int c, Mat& filter) {
    for (int y = 0; y < filter.rows; y++) for (int x = 0; x < filter.cols; x++) {

        int xx = x > filter.cols / 2 ? x - filter.cols : x;
        int yy = y > filter.rows / 2 ? y - filter.rows : y;
        float Duv = sqrtf(xx * xx + yy * yy);
        float Huv = (gH - gL) * (1 - exp(-c * Duv * Duv / (D0 * D0))) + gL;

        if (Duv < D0) {
            filter.at<Vec2f>(y, x)[0] = -0.5;
            filter.at<Vec2f>(y, x)[1] = -0.5;

        }
        else {
            filter.at<Vec2f>(y, x)[0] = Huv * 0.5 + 1;
            filter.at<Vec2f>(y, x)[1] = Huv * 0.5 + 1;
        }

    }
}

int main(int argc, const char* argv[])
{
    Mat f = imread("D:/Wiener_Input1.png", 0);
    f.convertTo(f, CV_32F, 1 / 255.f);
    Mat F;

    dft(f, F, DFT_COMPLEX_OUTPUT);
    //gaussian: 𝐻(𝑢,𝑣)=(𝛾_𝐻−𝛾_𝐿 )[1−𝑒^(−𝑐[(𝐷^2 (𝑢,𝑣))/(𝐷_0^2 )] ) ]+𝛾_𝐿
    /*
        for (auto y = 0; y < F.rows; y++) for (auto x = 0; x < F.cols; x++) {
            auto X = x;
            auto Y = y;
            if (X > F.cols / 2) X = F.cols - X;
            if (Y > F.rows / 2) Y = F.rows - Y;
            if (X * X - Y * Y > 3000) F.at<Vec2f>(y, x) = Vec2f(0, 0);
         }
         */
    Mat filter = Mat::zeros(F.size(), CV_32FC2);
    F = F + 0.1;
    log(F, F);
    dft(f, F, DFT_COMPLEX_OUTPUT);
    
    homofilter(1.0, 0.4, 3, 1, filter);

     //float x = filter.cols * F.rows / 10; // 크기 조정 가능

    std::vector<Mat> channels;
    //showFFT("filt", filter);
    waitKey();

    multiply(F, filter, F);      /// MULTIPLY 
    Mat g;
    idft(F, g, DFT_SCALE | DFT_REAL_OUTPUT);   /// inverse DFT
    
    exp(g, g);
    g = g - 0.1;
    /*for (int y = 0; y < g.rows; y++) for (int x = 0; x < g.cols; x++) {
        x = exp(x)-1;
        y = exp(y)-1;
        filter.at<Vec2f>(y, x)[0] = y;
        filter.at<Vec2f>(y, x)[1] = x;
    }
    */
    imshow("img", f);
    showFFT("fre", F);
    split(g, channels);
    imshow("res", channels[0]);
    return 0;
    //imshow("output", g);
    waitKey();
}

