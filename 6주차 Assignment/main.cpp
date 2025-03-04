#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

tuple<int, int> location01[4];
int count01 = 0;

tuple<int, int> location02[4];
int count02 = 0;

void CallBackFunc01(int event, int x, int y, int flags, void* param)
{
    Mat& img = *((Mat*)(param));
    Mat copy_img = img.clone();

    if (event == EVENT_LBUTTONDOWN && count01 < 4)
    {
        //cout << "좌표 = (" << x << ", " << y << ")" << endl;
        tuple<int, int> location = make_tuple(x, y);
        location01[count01] = location;

        int rect_width = 20;
        int rect_height = 20;
        rectangle(img, Point(x - rect_width / 2, y - rect_height / 2), Point(x + rect_width / 2, y + rect_height / 2), Scalar(0, 255, 0), 2, 1, 0);
        putText(img, to_string(count01), Point(x, y + 40), 0, 0.75, Scalar(255, 255, 255), 2, 1);
        imshow("img01", img);

        count01++;
    }

    if (count01 == 4)
    {
        for (int i = 0; i < 4; i++) {
            cout << "img01 좌표 " << i << " 번 (" << get<0>(location01[i]) << ", " << get<1>(location01[i]) << ")" << endl;
        }
        count01++;
    }
}

void CallBackFunc02(int event, int x, int y, int flags, void* param)
{
    Mat& img = *((Mat*)(param));
    Mat copy_img = img.clone();

    if (event == EVENT_LBUTTONDOWN && count02 < 4)
    {
        //cout << "좌표 = (" << x << ", " << y << ")" << endl;
        tuple<int, int> location = make_tuple(x, y);
        location02[count02] = location;

        int rect_width = 20;
        int rect_height = 20;
        rectangle(img, Point(x - rect_width / 2, y - rect_height / 2), Point(x + rect_width / 2, y + rect_height / 2), Scalar(0, 255, 0), 2, 1, 0);
        putText(img, to_string(count02), Point(x, y + 40), 0, 0.75, Scalar(255, 255, 255), 2, 1);
        imshow("img02", img);

        count02++;
    }

    if (count02 == 4)
    {
        for (int i = 0; i < 4; i++) {
            cout << "img02 좌표 " << i << " 번 (" << get<0>(location02[i]) << ", " << get<1>(location02[i]) << ")" << endl;
        }
        count02++;
    }
}


int main() {
    // src 이미지 로딩 및 축소
	Mat img1 = imread("img01.jpg", IMREAD_COLOR);
	resize(img1, img1, Size(img1.cols * 0.2, img1.rows * 0.2));

    // dst 이미지 로딩 및 축소
	Mat img2 = imread("img02.jpg", IMREAD_COLOR);
	resize(img2, img2, Size(img2.cols * 0.2, img2.rows * 0.2));

    Mat src, dst, dst2;
    img1.copyTo(src);
    img2.copyTo(dst); // 후방변환 + 양선형 보간
    img2.copyTo(dst2); // 후방변환만

    // src Window
    namedWindow("img01");
    setMouseCallback("img01", CallBackFunc01, &img1);
    imshow("img01", img1);

    // dst Window
    namedWindow("img02");
    setMouseCallback("img02", CallBackFunc02, &img2);
    imshow("img02", img2);
    
    waitKey(0);
    
    /** 호모그래피 계산 **/
    int x1 = get<0>(location01[0]);
    int y1 = get<1>(location01[0]);

    int x2 = get<0>(location01[1]);
    int y2 = get<1>(location01[1]);

    int x3 = get<0>(location01[2]);
    int y3 = get<1>(location01[2]);

    int x4 = get<0>(location01[3]);
    int y4 = get<1>(location01[3]);

    int x1_p = get<0>(location02[0]);
    int y1_p = get<1>(location02[0]);

    int x2_p = get<0>(location02[1]);
    int y2_p = get<1>(location02[1]);

    int x3_p = get<0>(location02[2]);
    int y3_p = get<1>(location02[2]);

    int x4_p = get<0>(location02[3]);
    int y4_p = get<1>(location02[3]);
    
    // 행렬 A 정의
    Mat A = Mat::zeros(8, 9, CV_32F);
    A.at<float>(0, 0) = -x1;
    A.at<float>(0, 1) = -y1;
    A.at<float>(0, 2) = -1;
    A.at<float>(0, 6) = x1 * x1_p;
    A.at<float>(0, 7) = y1 * x1_p;
    A.at<float>(0, 8) = x1_p;

    A.at<float>(1, 3) = -x1;
    A.at<float>(1, 4) = -y1;
    A.at<float>(1, 5) = -1;
    A.at<float>(1, 6) = x1 * y1_p;
    A.at<float>(1, 7) = y1 * y1_p;
    A.at<float>(1, 8) = y1_p;

    A.at<float>(2, 0) = -x2;
    A.at<float>(2, 1) = -y2;
    A.at<float>(2, 2) = -1;
    A.at<float>(2, 6) = x2 * x2_p;
    A.at<float>(2, 7) = y2 * x2_p;
    A.at<float>(2, 8) = x2_p;

    A.at<float>(3, 3) = -x2;
    A.at<float>(3, 4) = -y2;
    A.at<float>(3, 5) = -1;
    A.at<float>(3, 6) = x2 * y2_p;
    A.at<float>(3, 7) = y2 * y2_p;
    A.at<float>(3, 8) = y2_p;

    A.at<float>(4, 0) = -x3;
    A.at<float>(4, 1) = -y3;
    A.at<float>(4, 2) = -1;
    A.at<float>(4, 6) = x3 * x3_p;
    A.at<float>(4, 7) = y3 * x3_p;
    A.at<float>(4, 8) = x3_p;

    A.at<float>(5, 3) = -x3;
    A.at<float>(5, 4) = -y3;
    A.at<float>(5, 5) = -1;
    A.at<float>(5, 6) = x3 * y3_p;
    A.at<float>(5, 7) = y3 * y3_p;
    A.at<float>(5, 8) = y3_p;

    A.at<float>(6, 0) = -x4;
    A.at<float>(6, 1) = -y4;
    A.at<float>(6, 2) = -1;
    A.at<float>(6, 6) = x4 * x4_p;
    A.at<float>(6, 7) = y4 * x4_p;
    A.at<float>(6, 8) = x4_p;

    A.at<float>(7, 3) = -x4;
    A.at<float>(7, 4) = -y4;
    A.at<float>(7, 5) = -1;
    A.at<float>(7, 6) = x4 * y4_p;
    A.at<float>(7, 7) = y4 * y4_p;
    A.at<float>(7, 8) = y4_p;

    cout << A << endl;
    // DLT 이용 -> Ah = 0 을 푸는 문제
    Mat U, D, VT, V;

    // h를 구하려면 행렬 A에 대한 SVD를 구해야 함; U D VT = A 
    SVDecomp(A, U, D, VT, SVD::FULL_UV);

    // 행렬 VT를 전치하여 V 획득
    transpose(VT, V);

    // h(9X1)는 행렬 V의 오른쪽 끝 Row 벡터  
    Mat H_row = V.col(8);
    
    // 열 벡터 형태의 h를 3X3 사이즈의 H 행렬로 전환
    Mat H = Mat::ones(3, 3, CV_32F);
    H.at<float>(0, 0) = H_row.at<float>(0, 0);
    H.at<float>(0, 1) = H_row.at<float>(1, 0);
    H.at<float>(0, 2) = H_row.at<float>(2, 0);
    H.at<float>(1, 0) = H_row.at<float>(3, 0);
    H.at<float>(1, 1) = H_row.at<float>(4, 0);
    H.at<float>(1, 2) = H_row.at<float>(5, 0);
    H.at<float>(2, 0) = H_row.at<float>(6, 0);
    H.at<float>(2, 1) = H_row.at<float>(7, 0);
    H.at<float>(2, 2) = H_row.at<float>(8, 0);

    H = H / H.at<float>(2, 2);
    
    cout << "H 행렬:" << endl;
    cout << H << endl;


    // Target 이미지의 pixel이 Source 이미지에 어느 위치에 존재하는지 계산 (후방 기하 변환)
    // 범위 밖일 경우 무시하기 위함
    for (int y = 0; y < img2.rows; y++) {
        for (int x = 0; x < img2.cols; x++) {

            // Homogeneous Coordinate 이용
            Mat pixel_p = Mat::ones(3, 1, CV_32F);
            pixel_p.at<float>(0, 0) = x;
            pixel_p.at<float>(1, 0) = y;
            
            // H의 역행렬을 통해 X' -> X 변환
            Mat pixel = Mat::ones(3, 3, CV_32F);
            pixel = H.inv() * pixel_p;
            pixel = pixel / pixel.at<float>(2, 0);

            // CV32F 를 8bit U 정수로 변환
            int pixel_x = cvRound(pixel.at<float>(0, 0));
            int pixel_y = cvRound(pixel.at<float>(1, 0));
            

            // 변환된 픽셀(X)이 Source 이미지 안에 있는지 검사
            if ((pixel_x > 0) && (pixel_x < img1.cols) && (pixel_y > 0) && (pixel_y < img1.rows)) {
                //cout << pixel << endl;
                //cout << "pixel x : " << to_string(pixel_x) << endl;
                //cout << "pixel y : " << to_string(pixel_y) << endl;

                // Target 이미지의 픽셀 값 수정
                
                /** 후방 변환만 수행하는 코드 **/
                dst2.at<Vec3b>(y, x)[0] = src.at<Vec3b>(pixel_y, pixel_x)[0]; // 3채널의 B, G, R pixel 값을 각각 수정
                dst2.at<Vec3b>(y, x)[1] = src.at<Vec3b>(pixel_y, pixel_x)[1];
                dst2.at<Vec3b>(y, x)[2] = src.at<Vec3b>(pixel_y, pixel_x)[2];
                

                /** 후방변환 + 양선형 보간 코드 **/
                if (pixel_y < img2.rows - 1 && pixel_x < img2.cols - 1) {
                    // 인접한 두 픽셀과의 거리 계산
                    float a = pixel.at<float>(0, 0) - pixel_x;
                    float b = pixel.at<float>(1, 0) - pixel_y;
                    
                    // 2차원 보간식 유도
                    // f(y, x)
                    float f_yx_B = src.at<Vec3b>(pixel_y, pixel_x)[0];
                    float f_yx_G = src.at<Vec3b>(pixel_y, pixel_x)[1];
                    float f_yx_R = src.at<Vec3b>(pixel_y, pixel_x)[2];

                    // f(y,x') = (1-a) f(y,x) + a f(y,x+1)
                    float f_yx_p_B = ((1 - a) * f_yx_B) + (a * src.at<Vec3b>(pixel_y, pixel_x + 1)[0]);
                    float f_yx_p_G = ((1 - a) * f_yx_G) + (a * src.at<Vec3b>(pixel_y, pixel_x + 1)[1]);
                    float f_yx_p_R = ((1 - a) * f_yx_R) + (a * src.at<Vec3b>(pixel_y, pixel_x + 1)[2]);

                    // f(y+1,x') = (1-a) f(y+1, x) + a f(y+1, x+1)
                    float f_ya_x_p_B = ((1 - a) * src.at<Vec3b>(pixel_y + 1, pixel_x)[0]) + (a * (src.at<Vec3b>(pixel_y + 1, pixel_x + 1)[0]));
                    float f_ya_x_p_G = ((1 - a) * src.at<Vec3b>(pixel_y + 1, pixel_x)[1]) + (a * (src.at<Vec3b>(pixel_y + 1, pixel_x + 1)[1]));
                    float f_ya_x_p_R = ((1 - a) * src.at<Vec3b>(pixel_y + 1, pixel_x)[2]) + (a * (src.at<Vec3b>(pixel_y + 1, pixel_x + 1)[2]));

                    // f(y', x') = (1-b) f(y, x') + b f(y+1, x')
                    float f_y_p_x_p_B = ((1 - b) * f_yx_p_B) + (b * f_ya_x_p_B);
                    float f_y_p_x_p_G = ((1 - b) * f_yx_p_G) + (b * f_ya_x_p_G);
                    float f_y_p_x_p_R = ((1 - b) * f_yx_p_R) + (b * f_ya_x_p_R);



                    /*  (0~255) 값을 벗어나는 경우를 고려하지 못하는 교체                    
                    dst.at<Vec3b>(y, x)[0] = cvRound(f_y_p_x_p_B);
                    dst.at<Vec3b>(y, x)[1] = cvRound(f_y_p_x_p_G);
                    dst.at<Vec3b>(y, x)[2] = cvRound(f_y_p_x_p_R);
                    */

                    // (0~255) 값을 벗어나는 경우 0, 255로 값 고정
                    if (cvRound(f_y_p_x_p_B) > 255)
                        dst.at<Vec3b>(y, x)[0] = 255;
                    else if (cvRound(f_y_p_x_p_B) < 0)
                        dst.at<Vec3b>(y, x)[0] = 0;
                    else
                        dst.at<Vec3b>(y, x)[0] = cvRound(f_y_p_x_p_B);

                    if (cvRound(f_y_p_x_p_G) > 255)
                        dst.at<Vec3b>(y, x)[1] = 255;
                    else if (cvRound(f_y_p_x_p_G) < 0)
                        dst.at<Vec3b>(y, x)[1] = 0;
                    else
                        dst.at<Vec3b>(y, x)[1] = cvRound(f_y_p_x_p_G);

                    if (cvRound(f_y_p_x_p_R) > 255)
                        dst.at<Vec3b>(y, x)[2] = 255;
                    else if (cvRound(f_y_p_x_p_R) < 0)
                        dst.at<Vec3b>(y, x)[2] = 0;
                    else
                        dst.at<Vec3b>(y, x)[2] = cvRound(f_y_p_x_p_R);

                }
                // 오른쪽, 아래 모서리의 경우 보간 이용하지 않고 기존 Source 이미지의 픽셀 그대로 이용
                else {
                    dst.at<Vec3b>(y, x)[0] = src.at<Vec3b>(pixel_y, pixel_x)[0]; // 3채널의 B, G, R pixel 값을 각각 수정
                    dst.at<Vec3b>(y, x)[1] = src.at<Vec3b>(pixel_y, pixel_x)[1];
                    dst.at<Vec3b>(y, x)[2] = src.at<Vec3b>(pixel_y, pixel_x)[2];
                }
            }
        }
    }
    
    //cout << dst.type() << endl;
    
    //cout << dst.size << endl;
    //cout << img2.size << endl; 

    imshow("양선형보간", dst);
    imshow("후방변환", dst2);

    imwrite("result.jpg", dst);
    imwrite("result2.jpg", dst2);

    waitKey(0);
    
	return 0;
}