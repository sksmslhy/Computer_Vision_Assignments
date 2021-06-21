# SIFT를 참조한 Correlation 설정 실습	
3주차 Assignment에 대한 Code 제출물과 간략한 Review를 포함하고 있습니다.  
자세한 서술과 결과 정리는 Report에 포함되어 있습니다.


## Environment
각 팀원의 개발 및 테스트 환경은 아래와 같습니다.
| **Name** | **OS** | **OpenCV Library** | **Language** | **IDE** | 
|:--------:|:--------:|:--------:|:--------:|:--------:|
| 유승욱 | Windows 10 | OpenCV 4.5.1 | C++ | Visual Studio 2019  |
| 이나혁 | Mac OS 11 Big Sur | OpenCV 4.5.1 | C++ | Xcode 12.4 |
| 이하윤 | Mac OS 11 Big Sur | OpenCV 4.5.1 | C++ | Xcode 12.4 |

## Run
Code는 OpenCV 4.5.1버전을 통해 개발 및 테스트 되었으며, 원활한 동작을 위해 동일한 버전의 OpenCV를 사용하는 것을 추천드립니다.

```
$ g++ main.cpp -o app `pkg-config --cflags --libs opencv`
```

## Sample Result
샘플 결과는 아래와 같습니다. 파라미터값을 조절한 세부 결과는 Report에서 확인할 수 있습니다.
![](final_result.png)

## Code Review

### Header
OpenCV 함수 이용을 위한 highgui, imgproc 헤더와 C++ 자료구조 사용을 위한 tuple, string 헤더를 선언
```cpp
#include <iostream>
#include <tuple>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
```

### Calculate Image Histogram
Sobel Operator를 활용하여 영상 히스토그램 획득, 커널의 사이즈 수정
```cpp
Mat img2histImg(Mat img){
    
    Mat gx, gy;
    Sobel(img, gx, CV_32F, 1, 0, 3); 
    Sobel(img, gy, CV_32F, 0, 1, 3); // Sobel Operator 적용
    
    Mat mag, angle;
    cartToPolar(gx, gy, mag, angle, 1);  // 그라디언트의 크기와 방향 찾기
    
    Mat histMag, histAng;
    int histSize = 8; // 히스토그램 Bins 개수
    
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
    
    // 이후 과정 생략 (전체 내용 코드 첨부)
    
    return histImage;
}
```

### MouseCallBack Function
관심영역을 설정하고 Block 생성
```cpp
void CallBackFunc01(int event, int x, int y, int flags, void* param)
{
    Mat &img = *((Mat*)(param));
    Mat copy_img = img.clone();
    if (event == EVENT_LBUTTONDOWN && count01 < 4)
    {
        tuple<int, int> location = make_tuple(x, y);
        location01[count01] = location;
        int rect_width = 100; // Block 사이즈 설정
        int rect_height = 100; // Block 사이즈 설정
        rectangle(img, Point(x-rect_width/2, y-rect_height/2), Point(x+rect_width/2, y+rect_height/2), Scalar(255,255,255), 10, 8, 0);
        
        Mat subImage = copy_img(Range(y-rect_width/2, y+rect_width/2), Range(x-rect_width/2, x+rect_width/2));
        dotHistogram[count01] = img2histImg(subImage); // 관심영역 자르기
        
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
```

### Comparison of Histogram
히스토그램의 비교는 OpenCV에서 제공하는 지표를 활용, 1) Correlation, 2) Chi-Square, 3) Intersect를 이용하여 히스토그램의 비교를 수행하였고 수식과 코드는 아래와 같음
#### Using Correlation
```cpp
if (compareHist(histogram_img1_1, histogram_img2_1, HISTCMP_CORREL) >= result)
    {
        img01_1_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_CORREL);
    }
```

#### Using Chi-Square
```cpp
if (compareHist(histogram_img1_1, histogram_img2_1, HISTCMP_CHISQR) <= result)
    {
        img01_1_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_CHISQR);
    }
```

#### Using Intersect
```cpp
if (compareHist(histogram_img1_1, histogram_img2_1, HISTCMP_INTERSECT) >= result)
    {
        img01_1_to = 1;
        result = compareHist(histogram_img1_0, histogram_img2_1, HISTCMP_INTERSECT);
    }
```

### Visualization of final result
```cpp
line(img_concat, Point(get<0>(location01[3]), get<1>(location01[3])), Point(img01.cols + get<0>(location02[img01_3_to]), get<1>(location02[img01_3_to])), Scalar::all(0), 5, 8, 0);

imshow("Final Result", img_concat);
```
