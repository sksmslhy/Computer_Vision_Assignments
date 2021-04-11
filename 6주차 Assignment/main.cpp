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
        cout << "좌표 = (" << x << ", " << y << ")" << endl;
        tuple<int, int> location = make_tuple(x, y);
        location01[count01] = location;

        int rect_width = 20; //*** 여기 상자 사이즈 수정 ***
        int rect_height = 20; //*** 여기 상자 사이즈 수정 ***
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
        cout << "좌표 = (" << x << ", " << y << ")" << endl;
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




	return 0;
}