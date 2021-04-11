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

        int rect_width = 20; //*** 여기 상자 사이즈 수정 ***
        int rect_height = 20; //*** 여기 상자 사이즈 수정 ***
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
	Mat img1 = imread("img01.jpg", IMREAD_COLOR);
	resize(img1, img1, Size(img1.cols * 0.2, img1.rows * 0.2));

	Mat img2 = imread("img02.jpg", IMREAD_COLOR);
	resize(img2, img2, Size(img2.cols * 0.2, img2.rows * 0.2));


    namedWindow("img01");
    setMouseCallback("img01", CallBackFunc01, &img1);
    imshow("img01", img1);

    namedWindow("img02");
    setMouseCallback("img02", CallBackFunc02, &img2);
    imshow("img02", img2);

	waitKey(0);
	return 0;
}