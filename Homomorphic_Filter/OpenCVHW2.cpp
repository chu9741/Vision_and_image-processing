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
    //gaussian: 𝐻(𝑢,𝑣)=(𝛾_𝐻−𝛾_𝐿 )[1−𝑒^(−𝑐[(𝐷^2 (𝑢,𝑣))/(𝐷_0^2 )] ) ]+𝛾_𝐿
    /*for (int y = 0; y < filter.rows; y++) for (int x = 0; x < filter.cols; x++) {

        int xx = x > filter.cols / 2 ? x - filter.cols : x;
        int yy = y > filter.rows / 2 ? y - filter.rows : y;
        float Duv = sqrtf(xx * xx + yy * yy);
        float D0 = 3;
        float gH = 1.0;
        float gL = 0.4;
        int c = 1;
        float Huv = (gH - gL) * (1 - exp(-c * Duv * Duv / (D0 * D0))) + gL;

        if (Duv < D0) {
        filter.at<Vec2f>(y, x)[0] = -0.5;
        filter.at<Vec2f>(y, x)[1] = -0.5;

        }
        else{
           filter.at<Vec2f>(y, x)[0] = Huv*0.5+1;
           filter.at<Vec2f>(y, x)[1] = Huv*0.5+1;
        }

    }
    */
    homofilter(1.0, 0.4, 3, 1, filter);

    /*
        circle(filter, Point(0, 0), 50, Scalar(1, 1), -1); // 50 = frq 높을수록 선명히나옴
        circle(filter, Point(F.cols, F.rows), 50, Scalar(1, 1), -1);
        circle(filter, Point(F.cols, 0), 50, Scalar(1, 1), -1);
        circle(filter, Point(0, F.rows), 50, Scalar(1, 1), -1);
     */

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

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
