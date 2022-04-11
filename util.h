#pragma once
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <utility>

using std::vector;
using std::string;
using std::pair;

namespace util {

    struct Ratio {
        double value;
        double round;
        double floor;
        double ceil;
        Ratio() {
            update(0);
        }
        Ratio(double v) {
            update(v);
        }
        void update(double v) {
            value = v;
            round = std::round(v);
            floor = std::floor(v);
            ceil = std::ceil(v);
        }
    };

    vector<vector<double>> csvToVector(string pathToCSV);
    vector<Ratio> csvToRatioVector(string pathToCSV);
    void vectorToCSV(vector<vector<double>> vec, string pathToCSV);

    vector<vector<vector<double>>> copy3dVector(vector<vector<vector<double>>>& oldVector);

    pair<double, double>cartesianToSpherical(double arr[3]);
    double getEntropy(cv::Mat image);
    /*
        (Treemaker) pass in three parameters: perFrameHitCount, VPlength, VPheight, 
    */
    vector<vector<double>> syncDataFramerate(vector<vector<double>> data, double framerate);
    pair<double, double> treeMaker(double** map, const double length, const double height);

    vector<int> randPerm(int n, int size);

    //array utility functions

    void init_RatioArray(Ratio** address, int n);
    void init_DoubleArray(double** address, int n);

    void init_2dIntArray(int*** address, int rows, int cols);
    void init_2dRatioArray(Ratio*** address, int rows, int cols);
    void init_2dDoubleArray(double*** address, int rows, int cols);
}
