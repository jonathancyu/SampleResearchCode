#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "tests.h"
#include "Video.h"

using namespace std;
using namespace cv;

// Constants
const double rowNum = 24.0;
const double colNum = 36.0;
const double VPlength = 12;
const double VPheight = 12;
const double numOfTestUsers = 6;
const double scale = 6;
const double BlockRowNum = 5;
const double BlockColNum = 6;
const double numOfViewers = 0;
const double numOfTiles = rowNum * colNum;

const string videoPath = "C:/Users/yucjo/Documents/2021 summer/DSLab/Videos/original/";
const string tracePath = "C:/Users/yucjo/Documents/2021 summer/DSLab/Traces/VideoID/";

int main() {
	tests t = tests();
	t.run();

    while (1);
	return 0;
}