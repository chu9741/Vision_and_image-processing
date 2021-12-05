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

/*이번 과제는 Wiener Filter를 이해하고 활용하는 것입니다.
첨부된 Wiener_input1과 Wiener_input2는 각각 Wiener_kernel에 의해 degradation된 영상, Wiener_kernel과 uniform noise에 의해 degradation된 영상입니다. (input1역시 quantization error를 포함하고 있습니다.)
이들을 Wiener filter를 이용하여 복원하는 것입니다.

2. 내용
a. input1과 kernel(point spread function)을 읽거 각각을 DFT 합니다. G, H input이 G인가,,,? 그러면 말이 되긴하는데
b. Inverse filtering을 이용하여 복원하고 결과를 확인합니다. (Wiener filter에서 K를 0으로 하는 것과 동일합니다.) F = G/H
c. Wiener filter를 이용하여 복원하고 결과를 확인합니다. (K>0, 예를 들면 0.001~0.01 정도로 지정합니다.)
d. 동일한 과정을 input2에 적용하여 결과를 확인합니다.
e. 과제 제출은 input2에 대하여 wiener filter를 적용한 상태에서 제출합니다. (소스코드만 제출합니다.)

3. 주의 사항
a. 각각 영상은 dft를 위해 32bit float로 convert한 후 과정을 진행합니다.
b. 사용된 모든 영상은 앞에서 사용한 수식으로 만든 Kernel이 아니므로 복소수이고, 따라서 곱셈, 나눗셈에 별도의 주의가 필요합니다.
   1) 복소수 영상의 곱셈은 mulSpectrum( input1, input2, output, 0 ) 과 같이 할 수 있습니다.   mulSpectrum( H, G , F, 0) 이건 GH* (분자계산)
   2) 복소수 영상의 나눗셈은 훨씬 복잡합니다.
       point spread function을 DFT 한 결과를 H라고 할 때, H의 한 픽셀은 a+bi 입니다.
       a+bi로 X(역시 복소수)를 나누는 것은 X/(a+bi)로 나타나고, 분모의 유리화를 통해 X*(a-bi)/(a*a+b*b) 로 나타납니다.
       즉 G/H는 (G x H*) / |H|2 과 같이 표현됩니다.
       분자 부분은 mulSpectrum( G, H, output, 0, true ) 의 방법으로 계산 가능합니다. (마지막의 true가 H를 conjugate로 만들어 곱해 줍니다.)
       분모 부분은 showFFT 함수에서  magnitude 계산 방법으로 계산 가능합니다. magnitude(Mat , Mat , Mat )
       계산된 magnitude 는 |H|이므로, 제곱하여 |H|2 (이후 D로 표현)를 계산할 수 있습니다. Mat.mul() = 원소끼리 곱 , Mat*Mat = 행렬곱

       분자를 분모로 나눌때 D가 real 만 있는 one channel이므로 주의가 필요합니다. a*a + b*b
       (c+di)/R = c/R + d/Ri 이므로 두 channel 모두 D가 되어 있는 두 channel image를 만듭니다. (merge 함수를 사용합니다.)
       최종적으로 나눗셈은 divide 함수를 사용합니다.
c. Wiener filter를 구현하는 과정에서는 b-2)에 소개된 식대로 복소수 나눗셈을 전개 한 후, 강의자료의 원식을 변형하면 쉽게 해결할 수 있습니다.
*/