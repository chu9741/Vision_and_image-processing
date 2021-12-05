// VisionOpenCVHW1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;
//burt_mask.png
const int PYR_LEVEL = 10;
vector<Mat> buildGaussianPyramid(const Mat &src) {
    vector<Mat> GaussianPyramid(PYR_LEVEL);
    GaussianPyramid[0] = src;
    for (auto i = 1; i < PYR_LEVEL; i++) {
        pyrDown(GaussianPyramid[i-1], GaussianPyramid[i]);
    }
    return GaussianPyramid;
}
/*
vector<Mat> buildGaussianPyramid2(const Mat& src) {
    vector<Mat> GaussianPyramid(PYR_LEVEL);
    GaussianPyramid[0] = 1-src;
    for (auto i = 1; i < PYR_LEVEL; i++) {
        pyrDown(GaussianPyramid[i - 1], GaussianPyramid[i]);
    }
    return GaussianPyramid;
}
*/
vector<Mat> buildLaplacianPyramid(const Mat& src) {
    vector<Mat> Pyramid(PYR_LEVEL);
    Pyramid[0] = src;
    for (auto i = 1; i < PYR_LEVEL; i++) {
        Mat temp;
        pyrDown(Pyramid[i-1], Pyramid[i]);  //가우시안 블러 하고 이미지 줄이기
        pyrUp(Pyramid[i], temp, Pyramid[i - 1].size() ); // 키우고 원본(src)에서 뺸다,,,?
        Pyramid[i-1] -= temp; // 원래 소스에서 빼는과정 
    }
    return Pyramid;
}

Mat reconstruct(const std::vector<Mat>& Pyramid){  //피라미드로 나뉜 이미지를 합쳐서 원본으로 만듬
    Mat Out=Pyramid.back(); // 제일 낮은부분부터 더해서 올라감
    //끝에잇는거부터 두배 키워서 더하고 , 또 두배키워서 더하고,,,
    for (int i = int(Pyramid.size()) - 2;  i >= 0; i--) {
        pyrUp(Out, Out, Pyramid[i].size());
        Out += Pyramid[i];
    }
    return Out;
}

int main(int argc, const char *argv[])
{
    Mat orange = imread("D:/burt_orange.png"); 
    Mat mask = imread("D:/burt_mask.png");
    Mat apple = imread("D:/burt_apple.png");
    Mat orangesrc;    // 
    Mat applesrc;
    Mat masksrc;
    Mat masksrc2;

    mask.convertTo(masksrc, CV_32F, 1 / 255.f);
    mask.convertTo(masksrc2, CV_32F, 1 / 255.f);   
    orange.convertTo(orangesrc, CV_32F, 1 / 255.f);
    apple.convertTo(applesrc, CV_32F, 1 / 255.f);
    flip(masksrc, masksrc2, 1); // mask를 뒤집어서 mask2를 만듬 

    vector<Mat>OLPyramid = buildLaplacianPyramid(orangesrc); // Orange img Lapalcian
    vector<Mat>GPyramid = buildGaussianPyramid(masksrc);  // mask img Gaussian
    vector<Mat>G2Pyramid = buildGaussianPyramid(masksrc2); // flipped mask img Gaussian
    vector<Mat>ALPyramid = buildLaplacianPyramid(applesrc); // Apple img Laplacian
    vector<Mat>output;
    
    for (auto i = 0; i < ALPyramid.size(); i++) {
        multiply(ALPyramid[i], GPyramid[i], ALPyramid[i]);
        //imshow("apple" + to_string(i), ALPyramid[i] + Scalar(0.5, 0.5, 0.5));
        multiply(OLPyramid[i], G2Pyramid[i], OLPyramid[i]);
        //imshow("apple2" + to_string(i), OLPyramid[i] + Scalar(0.5, 0.5, 0.5));
        add(OLPyramid[i], ALPyramid[i], OLPyramid[i]);
        //imshow("apple" + to_string(i), OLPyramid[i]+ ((i<OLPyramid.size()-1)?Scalar(0.5, 0.5, 0.5):Scalar(0)) ); // Scalar 값은 이미지의 음수 부분이 보이게 하기위해 넣음 RGB값, 
    }
    Mat res = reconstruct(OLPyramid); // orange + apple 
    //Mat res2 = reconstruct(ALPyramid);
    imshow("res", res);
    //imshow("res2", res2);
    waitKey();
    return 0;
}

