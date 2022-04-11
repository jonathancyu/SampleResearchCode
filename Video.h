//
// Created by yucjo on 7/2/2021.
//
#pragma once

#ifndef C_VIDEO_H
#define C_VIDEO_H

#include "util.h"
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
#include <utility>

using std::vector;
using std::string;
using std::pair;

using namespace util;

class Video {
public:
    int videoID;
    cv::VideoCapture capture;
    int frameCount; 

    //todo: trace class? this is UGLY
    vector<vector<vector<double>>> traces;
    double framerate;

    double* frameEntropy;
    double* tileHitCount;

    //possibly arrays of varying classes
    double* perFrameTrees;
    double* perFrameTreeWeights;
    double* numOfParties;
    double* tileHitPercent;
    Ratio* perFrameNumOfParties;
    double* perFrameTreeViewport;

    Ratio* HitRatioPercent;
    Ratio* BigHitRatioPercent;
    Ratio* HitRatio;
    Ratio* BigHitRatio;
    //new..?
    Ratio* HitRatioPercentNew;
    Ratio* BigRatioPercentNew;
    Ratio* HitRatioNew;
    Ratio* BigHitRatioNew;

    //others
    Ratio* BigNumOfTilesSent;
    Ratio* NumOfTilesSent;


    Video(string videoPath, int id);

    void process(string tracePath);

    void init_arrays(int n);
    double** tileHitCounter(vector<pair<double, double>> userSamples, double height, double width, double tileHeight,
        double tileLength);
    void processFrame(cv::Mat frame, int frameNumber);


    /*
video = VideoReader(strcat(videoPath,int2str(vidID),'.mp4'));
numOfFrames = round(video.FrameRate*video.Duration);
BUCKETIZEINTERVAL = 1/video.FrameRate;
roundHitRatioPercentNew = zeros(numOfFrames,1);
roundBigHitRatioPercentNew = zeros(numOfFrames,1);
roundHitRatioNew = zeros(numOfFrames,1);
roundBigHitRatioNew = zeros(numOfFrames,1);
floorHitRatioPercentNew = zeros(numOfFrames,1);
floorBigHitRatioPercentNew = zeros(numOfFrames,1);
floorHitRatioNew = zeros(numOfFrames,1);
floorBigHitRatioNew = zeros(numOfFrames,1);
ceilingHitRatioPercentNew = zeros(numOfFrames,1);
ceilingBigHitRatioPercentNew = zeros(numOfFrames,1);
ceilingHitRatioNew = zeros(numOfFrames,1);
ceilingBigHitRatioNew = zeros(numOfFrames,1);
roundBigNumOfTilesSent = zeros(numOfFrames,1);
roundNumOfTilesSent = zeros(numOfFrames,1);
floorBigNumOfTilesSent = zeros(numOfFrames,1);
floorNumOfTilesSent = zeros(numOfFrames,1);
ceilingBigNumOfTilesSent = zeros(numOfFrames,1);
ceilingNumOfTilesSent = zeros(numOfFrames,1);*/


};


#endif //C_VIDEODATA_H
