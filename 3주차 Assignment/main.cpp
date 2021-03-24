#include <iostream>
#include <tuple>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

tuple<int, int> location01[4];
int count01 = 0;

tuple<int, int> location02[4];
int count02 = 0;

// 히스토그램 정보를 저장하는 배열
Mat dotHistogram[8];

// 1 Channel 이미지를 받아 histImg를 반환하는 함수
Mat img2histImg(Mat img){

    // Sobel Operator 적용
    Mat gx, gy;
    Sobel(img, gx, CV_32F, 1, 0, 3); // 가로 미분, 커널 사이즈 3 *** 여기 커널 사이즈 수정 ***
    Sobel(img, gy, CV_32F, 0, 1, 3); // 세로 미분, 커널 사이즈 3 *** 여기 커널 사이즈 수정 ***
    
    // 그라디언트의 크기와 방향 찾기
    Mat mag, angle;
    cartToPolar(gx, gy, mag, angle, 1);
    //imshow("그라디언트 크기 영상 : ", mag);
    //imshow("그라디언트 방향 영상 :", angle);
    
    Mat histMag, histAng;
    // number of bins of the histogram
    int histSize = 8; //*** 여기 Bins 개수 수정 ***
    
    // degrees goes from 0-360 if radians then change acordingly
    float rangeAng[] = {0, 360};
    const float* histRangeAng = {rangeAng};
    
    // Magnitude의 최소, 최대값 획득
    double minval, maxval;
    minMaxLoc(mag, &minval, &maxval);
    
    
    float rangeMag[] = { static_cast<float>(minval), static_cast<float>(maxval)} ;
    const float* histRangeMag = { rangeMag };
    
    calcHist(&mag, 1, 0, Mat(), histMag, 1, &histSize, &histRangeMag, true, false);
    calcHist(&angle, 1, 0, Mat(), histAng, 1, &histSize, &histRangeAng, true, false);
    
    // Draw the histograms
    int hist_w = 200; int hist_h = 200;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

    // Normalize the result to [ 0, histImage.rows ]
    normalize(histAng, histAng, 0, histImage.rows, cv::NORM_MINMAX, -1, Mat() );

    // Draw the lines
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histAng.at<float>(i-1)) ) ,
             Point(bin_w*(i), hist_h - cvRound(histAng.at<float>(i))),
             Scalar(0, 0, 0), 2, 8, 0);
    }
    
    return histImage;
}

void CallBackFunc01(int event, int x, int y, int flags, void* param)
{
    Mat &img = *((Mat*)(param));
    Mat copy_img = img.clone();
    if (event == EVENT_LBUTTONDOWN && count01 < 4)
    {
        //cout << "좌표 = (" << x << ", " << y << ")" << endl;
        tuple<int, int> location = make_tuple(x, y);
        location01[count01] = location;
        int rect_width = 100; //*** 여기 상자 사이즈 수정 ***
        int rect_height = 100; //*** 여기 상자 사이즈 수정 ***
        rectangle(img, Point(x-rect_width/2, y-rect_height/2), Point(x+rect_width/2, y+rect_height/2), Scalar(255,255,255), 10, 8, 0);
        
        // 관심영역 자르기
        Mat subImage = copy_img(Range(y-rect_width/2, y+rect_width/2), Range(x-rect_width/2, x+rect_width/2));
        dotHistogram[count01] = img2histImg(subImage);
        
        putText(img, to_string(count01), Point(x, y + 150), 0, 3, Scalar(255, 255, 255), 5, 1);
        imshow("img01", img);
        count01++;
    }
    
    if (count01 == 4)
    {
        for (int i = 0; i < 4; i++){
            cout << "img01 좌표 " << i << " 번 (" << get<0>(location01[i]) << ", " << get<1>(location01[i]) << ")" << endl;
            string number01 = "img01 Histogram " + to_string(i);
            namedWindow(number01);
            imshow(number01, dotHistogram[i]);
        }
        count01++;
    }
}

void CallBackFunc02(int event, int x, int y, int flags, void* param)
{
    Mat &img = *((Mat*)(param));
    Mat copy_img = img.clone();
    
    if (event == EVENT_LBUTTONDOWN && count02 < 4)
    {
        //cout << "좌표 = (" << x << ", " << y << ")" << endl;
        tuple<int, int> location = make_tuple(x, y);
        location02[count02] = location;
        int rect_width = 100;
        int rect_height = 100;
        rectangle(img, Point(x-rect_width/2, y-rect_height/2), Point(x+rect_width/2, y+rect_height/2), Scalar(255,255,255), 10, 8, 0);
        
        // 관심영역 자르기
        Mat subImage = copy_img(Range(y-rect_width/2, y+rect_width/2), Range(x-rect_width/2, x+rect_width/2));
        dotHistogram[count02+4] = img2histImg(subImage);
        
        putText(img, to_string(count02), Point(x, y + 150), 0, 3, Scalar(255, 255, 255), 5, 1);
        imshow("img02", img);
        count02++;
    }
    
    if (count02 == 4)
    {
        for (int i = 0; i < 4; i++){
            cout << "img02 좌표 " << i << " 번 (" << get<0>(location02[i]) << ", " << get<1>(location02[i]) << ")" << endl;
            string number02 = "img02 Histogram " + to_string(i);
            namedWindow(number02);
            imshow(number02, dotHistogram[i+4]);
        }
        count02++;
    }
}


int main() {
    
    // 이미지 불러오기
    Mat img01 = imread("1st.jpg", IMREAD_GRAYSCALE);
    Mat img02 = imread("2nd.jpg", IMREAD_GRAYSCALE);
    
    // img01에 대한 Window, MouseCallback, imshow
    namedWindow("img01");
    setMouseCallback("img01", CallBackFunc01, &img01);
    imshow("img01", img01);
    
    // img02에 대한 Window, MouseCallback, imshow
    namedWindow("img02");
    setMouseCallback("img02", CallBackFunc02, &img02);
    imshow("img02", img02);
    
    // 총 8개의 점이 찍히고 다음 키가 눌릴 때까지 대기
    waitKey(0);
    
    // converTo() 함수 사용을 위해 CV_8U에서 CV_32F로 변환, 정규화
    Mat histogram_img1_0, histogram_img1_1, histogram_img1_2, histogram_img1_3;
    Mat histogram_img2_0, histogram_img2_1, histogram_img2_2, histogram_img2_3;
    
    dotHistogram[0].convertTo(histogram_img1_0, CV_32F, 1/255.0);
    dotHistogram[1].convertTo(histogram_img1_1, CV_32F, 1/255.0);
    dotHistogram[2].convertTo(histogram_img1_2, CV_32F, 1/255.0);
    dotHistogram[3].convertTo(histogram_img1_3, CV_32F, 1/255.0);
    dotHistogram[4].convertTo(histogram_img2_0, CV_32F, 1/255.0);
    dotHistogram[5].convertTo(histogram_img2_1, CV_32F, 1/255.0);
    dotHistogram[6].convertTo(histogram_img2_2, CV_32F, 1/255.0);
    dotHistogram[7].convertTo(histogram_img2_3, CV_32F, 1/255.0);
    
    Mat img_concat;
    hconcat(img01, img02, img_concat);
    
    double result = 0.0;
    
    int img01_0_to = 0;
    int img01_1_to = 0;
    int img01_2_to = 0;
    int img01_3_to = 0;

    result = 0.0;
    if (compareHist(histogram_img1_0, histogram_img2_0, HISTCMP_INTERSECT) >= result)
    {
        img01_0_to = 0;
        result = compareHist(histogram_img1_0, histogram_img2_0, HISTCMP_INTERSECT);
    }
    
    if (compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_INTERSECT) >= result)
    {
        img01_0_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_INTERSECT);
    }
    if (compareHist(histogram_img1_0, histogram_img2_2, HISTCMP_INTERSECT) >= result)
    {
        img01_0_to = 2;
        result = compareHist(histogram_img1_0, histogram_img2_2, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_0, histogram_img2_3, HISTCMP_INTERSECT) >= result)
    {
        img01_0_to = 3;
        result = compareHist(histogram_img1_0, histogram_img2_3, HISTCMP_INTERSECT);
    }
        
    cout << "Img01's Dot 0 to Img02's Dot " << to_string(img01_0_to) << endl;
    line(img_concat, Point(get<0>(location01[0]), get<1>(location01[0])), Point(img01.cols + get<0>(location02[img01_0_to]), get<1>(location02[img01_0_to])), Scalar::all(0), 5, 8, 0);
    
    result = 0.0;
    if (compareHist(histogram_img1_1, histogram_img2_0, HISTCMP_INTERSECT) >= result)
    {
        img01_1_to = 0;
        result = compareHist(histogram_img1_0, histogram_img2_0, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_1, histogram_img2_1, HISTCMP_INTERSECT) >= result)
    {
        img01_1_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_1, histogram_img2_2, HISTCMP_INTERSECT) >= result)
    {
        img01_1_to = 2;
        result = compareHist(histogram_img1_0, histogram_img2_2, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_1, histogram_img2_3, HISTCMP_INTERSECT) >= result)
    {
        img01_1_to = 3;
        result = compareHist(histogram_img1_0, histogram_img2_3, HISTCMP_INTERSECT);
    }
        
    cout << "Img01's Dot 1 to Img02's Dot " << to_string(img01_1_to) << endl;
    line(img_concat, Point(get<0>(location01[1]), get<1>(location01[1])), Point(img01.cols + get<0>(location02[img01_1_to]), get<1>(location02[img01_1_to])), Scalar::all(0), 5, 8, 0);
    
    result = 0.0;
    if (compareHist(histogram_img1_2, histogram_img2_0, HISTCMP_INTERSECT) >= result)
    {
        img01_2_to = 0;
        result = compareHist(histogram_img1_0, histogram_img2_0, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_2, histogram_img2_1, HISTCMP_INTERSECT) >= result)
    {
        img01_2_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_2, histogram_img2_2, HISTCMP_INTERSECT) >= result)
    {
        img01_2_to = 2;
        result = compareHist(histogram_img1_0, histogram_img2_2, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_2, histogram_img2_3, HISTCMP_INTERSECT) >= result)
    {
        img01_2_to = 3;
        result = compareHist(histogram_img1_0, histogram_img2_3, HISTCMP_INTERSECT);
    }
        
    cout << "Img01's Dot 2 to Img02's Dot " << to_string(img01_2_to) << endl;
    line(img_concat, Point(get<0>(location01[2]), get<1>(location01[2])), Point(img01.cols + get<0>(location02[img01_2_to]), get<1>(location02[img01_2_to])), Scalar::all(0), 5, 8, 0);
    
    result = 0.0;
    if (compareHist(histogram_img1_3, histogram_img2_0, HISTCMP_INTERSECT) >= result)
    {
        img01_3_to = 0;
        result = compareHist(histogram_img1_0, histogram_img2_0, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_3, histogram_img2_1, HISTCMP_INTERSECT) >= result)
    {
        img01_3_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_3, histogram_img2_2, HISTCMP_INTERSECT) >= result)
    {
        img01_3_to = 2;
        result = compareHist(histogram_img1_0, histogram_img2_2, HISTCMP_INTERSECT);
    }
        
    if (compareHist(histogram_img1_3, histogram_img2_3, HISTCMP_INTERSECT) >= result)
    {
        img01_3_to = 3;
        result = compareHist(histogram_img1_0, histogram_img2_3, HISTCMP_INTERSECT);
    }
        
    cout << "Img01's Dot 3 to Img02's Dot " << to_string(img01_3_to) << endl;
    line(img_concat, Point(get<0>(location01[3]), get<1>(location01[3])), Point(img01.cols + get<0>(location02[img01_3_to]), get<1>(location02[img01_3_to])), Scalar::all(0), 5, 8, 0);

    imshow("Final Result", img_concat);

    
    waitKey(0);
    destroyAllWindows();
    return 0;
}
