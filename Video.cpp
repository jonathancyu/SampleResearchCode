//
// Created by yucjo on 7/2/2021.
//

#include "Video.h"
#include "Constants.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace util;

// Constants
Video::Video(string videoPath, int id) {
    videoID = id;
    capture = cv::VideoCapture();
    capture.open(videoPath + std::to_string(id) + ".mp4");
    framerate = capture.get(cv::CAP_PROP_FPS);
    frameCount = capture.get(cv::CAP_PROP_FRAME_COUNT);
    init_arrays(frameCount);
}

void Video::process(string tracePath) {
    for (auto& trace : std::filesystem::directory_iterator(tracePath + std::to_string(videoID))) {
        std::filesystem::path fileName = trace.path().filename();
        if (fileName == "2x2" || fileName == "2x3") continue;

        vector<vector<double>> newEntry = csvToVector(trace.path().string());
        traces.push_back(newEntry);
    }
    for (int i = 0; i < frameCount; i++) {
        cv::Mat frame;
        capture.read(frame);
        if (!frame.empty()) {
            processFrame(frame, i);
        } else {
            std::cout << "ERROR: FRAME " << i << " IS EMPTY" << std::endl;
        }
    }
}

//TODO: function may have index issues (matlab indexes at 1)
double** Video::tileHitCounter(vector<pair<double, double>> userSamples, double height, double width, double tileHeight, double tileWidth) {
    double** hitCount;
    init_2dDoubleArray(&hitCount, rowNum, colNum);
    for (pair<double, double> sample : userSamples) {
        if (sample.first == 400) continue;
        double tileWeight = 1.0;
        double theta = sample.first + 180.0;  //change range from [-180, 180] to [0, 360]
        double phi = sample.second + 90.0;    //change range from [-90, 90] to [0, 180]
        theta = theta * width / 360.0;
        phi = phi * height / 180.0;

        int thetaTile = floor(theta / tileWidth);
        int phiTile = floor(phi / tileHeight);

        hitCount[phiTile][thetaTile] += tileWeight;

        for (int i = 1; i <= scale; i++) {
            tileWeight = 1 / (i + 1.0);
            int thetaRightTile = thetaTile + i;
            int thetaLeftTile = thetaTile - i;
            int phiUpTile = phiTile + i;
            int phiDownTile = phiTile - i;
            if (thetaTile >= colNum) {
                thetaTile = thetaRightTile - colNum;
            }
            if (thetaLeftTile < 0)
                thetaLeftTile = thetaLeftTile + colNum;
            if (phiUpTile >= rowNum)
                phiUpTile = phiUpTile - rowNum;
            if (phiDownTile < 0)
                phiDownTile = phiDownTile + rowNum;
            if (thetaRightTile < thetaLeftTile) {
                thetaRightTile = thetaRightTile + colNum;
            }

            for (int col = thetaLeftTile; col <= thetaRightTile; col++) {
                int i = col % colNum;
                hitCount[phiUpTile][i] = hitCount[phiUpTile][i] + tileWeight;
                hitCount[phiDownTile][i] = hitCount[phiDownTile][i] + tileWeight;
            }
            thetaRightTile = thetaRightTile % colNum;
            phiDownTile = phiDownTile % colNum;
            // doesnt seem necessary
            // phiUpTile = phiUpTile - 1;
            if (phiUpTile == 0) {
                phiUpTile = rowNum;
            }
            if (phiUpTile < phiDownTile) {
                phiUpTile += rowNum;
            }
            //TODO: potential area for incorrect wrapping - going all the way to the top will put you at the bottom
            for (int row = phiDownTile; row <= phiUpTile; ++row) {
                int temp = row % rowNum;
                hitCount[temp][thetaLeftTile] = hitCount[temp][thetaLeftTile] + tileWeight;
                hitCount[temp][thetaRightTile] = hitCount[temp][thetaRightTile] + tileWeight;
            }
        }
    }
    return hitCount;
}


void Video::processFrame(cv::Mat frame, int frameNumber) {
    int rounds = 0;
    //loop doesn't terminate til line 700 of Main.m -_-
    while (rounds == 0 || abs(HitRatioNew->round - HitRatio->round) > 0.05) {
        rounds++;
        Ratio* hitCount;
        Ratio* hitPercent;
        Ratio* bigHitCount;
        Ratio* bigHitPercent;
        init_RatioArray(&hitCount, frameCount);
        init_RatioArray(&hitPercent, frameCount);
        init_RatioArray(&bigHitCount, frameCount);
        init_RatioArray(&bigHitPercent, frameCount);
        vector<int> testUsers = randPerm(traces.size(), floor(traces.size()) * 0.2); //grab a random fifth of test data
        //vector<int> testUsers = { 16, 29, 24, 26, 18, 11 }; // test line, replace with above 
        vector<vector<vector<double>>> trainingData = copy3dVector(traces);
        vector<vector<vector<double>>> testData;
        for (int i : testUsers) {
            if (i < trainingData.size()) {
                trainingData[i].clear();
            }
            testData.push_back(traces[i]);
        }
        //pick up at line 120
        int numOfViewers = trainingData.size();
        // TODO: Rename userSamples
        vector<pair<double, double>> userSamples(numOfViewers);

        double Fentropy = getEntropy(frame); // gave -nan(ind)
        double height = frame.rows;
        double width = frame.cols;

        double tileHeight = height / rowNum;
        double tileWidth = width / colNum;

        // 131
        numOfViewers = 0;
        for (int viewer = 0; viewer < trainingData.size(); ++viewer) {
            if (trainingData[viewer].size() < frameNumber) {
                pair<double, double> lookAngle = { 400.0, 400.0 };
                userSamples[viewer] = lookAngle;
                continue;
            } else if (trainingData[viewer].empty()) {
                continue;
            }
            numOfViewers++;
            auto currentSample = trainingData[viewer][frameNumber];
            userSamples[viewer] = { currentSample[1], currentSample[2] };        
        }
        // 143

        double** perFrameHitPercent;
        init_2dDoubleArray(&perFrameHitPercent, rowNum, colNum);

        double** perFrameHitCount = tileHitCounter(userSamples, height, width, tileHeight, tileWidth);

        // TODO: CHECK THIS Need to see tileHitCount data type, 
        vector<double**>tileHitCount(frameNumber + 1);
        tileHitCount[frameNumber] = perFrameHitCount;
        
        // 148: Treemaker starts

    }
}


//ex: video is at 25fps, data is collected at 60 fps. returns data sampled at 25 fps.

//TODO: make separate matlab function class
// Makes array all 0s


void Video::init_arrays(int n) {
    init_DoubleArray(&perFrameTrees, n);
    init_DoubleArray(&perFrameTreeWeights, n);
    init_DoubleArray(&numOfParties, n);
    init_DoubleArray(&tileHitPercent, n);
    init_RatioArray(&perFrameNumOfParties, n);
    init_DoubleArray(&perFrameTreeViewport, n);

    init_RatioArray(&HitRatioPercent, n);
    init_RatioArray(&BigHitRatioPercent, n);
    init_RatioArray(&HitRatio, n);
    init_RatioArray(&BigHitRatio, n);
    init_RatioArray(&HitRatioPercentNew, n);
    init_RatioArray(&BigRatioPercentNew, n);
    init_RatioArray(&HitRatioNew, n);
    init_RatioArray(&BigHitRatioNew, n);

    init_RatioArray(&BigNumOfTilesSent, n);
    init_RatioArray(&NumOfTilesSent, n);
}


