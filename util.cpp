#include "util.h"
#include "Constants.h"
#include <fstream>
#include <filesystem>
#include <random>

pair<double, double> util::cartesianToSpherical(double arr[3]) {
    double x = arr[0];
    double y = arr[1];
    double z = arr[2];

    // Converting x,y,z from cartesian coordinates to spherical coordinates
    double t = atan2(y, x);
    double p = atan2(z, sqrt(x * x + y * y));
    double r = sqrt(x * x + y * y + z * z);

    // Converting from radians to degrees
    double theta = t * 180.0 / PI;
    double phi = p * 180.0 / PI;

    pair<double, double> answer = { theta, phi };
    return answer;
}

vector<vector<double>> util::csvToVector(string pathToCSV) {
    vector<vector<double>> answer;
    string line;
    std::ifstream infile;
    infile.open(pathToCSV);
    while (getline(infile, line)) {
        vector<double>temp;
        std::stringstream ss(line);
        string x;
        while (std::getline(ss, x, ',')) {
            temp.push_back(std::stod(x));
        }
        answer.push_back(temp);
    }
    return answer;
}

vector<util::Ratio> csvToRatioVector(string pathToCSV) {
    vector<util::Ratio> answer;
    string line;
    std::ifstream infile;
    infile.open(pathToCSV);
    while (getline(infile, line)) {
        vector<double>temp;
        std::stringstream ss(line);
        string x;
        std::getline(ss, x, ',');
        answer.push_back(util::Ratio(std::stod(x)));
    }
    return answer;
}

vector<vector<vector<double>>> util::copy3dVector(vector<vector<vector<double>>>& oldVector) {
    vector<vector<vector<double>>> newVector;
    for (vector<vector<double>> row : oldVector) {
        vector<vector<double>> newRow;
        for (vector<double> col : row) {
            newRow.emplace_back(vector<double>(col));
        }
        newVector.push_back(newRow);
    }
    return newVector;
}

/* TODO: maybe encapsulate into a Frame class?
 *     : does not give the same values as Matlab *
 *          expected | given                     *
 *  img1    8        | 5.54176                   *
 *  img2    7.9951   | 5.54518                   */

double util::getEntropy(cv::Mat img) {
    if (img.channels() == 3) { //if colored, convert to grayscale
        cv::cvtColor(img, img, cv::COLOR_RGB2GRAY);
    }
    const int histSize = 256;
    cv::Mat hist = cv::Mat(histSize, histSize, CV_8UC1, cv::Scalar(0));
    float range[] = { 0, 256 };
    const int* channels = { 0 };
    const float* ranges = { range };
    const cv::Mat* f = &img;
    cv::calcHist(f, 1, channels, cv::Mat(), hist, 1, &histSize, &ranges, true, false);
    hist /= img.total();
    //hist += 1e-4; // "prevent zero"
    cv::Mat logP;
    cv::log(hist, logP);
    return -1 * cv::sum(hist.mul(logP)).val[0];
}


vector<int> util::randPerm(int n, int size) {
    vector<int> result;
    for (int i = 0; i < n; i++) result.push_back(i);
    shuffle(result.begin(), result.end(), std::default_random_engine());
    result.resize(size);
    return result;
}

//array utility functions
void util::init_DoubleArray(double** address, int n) {
    (*address) = new double[n];
    for (int i = 0; i < n; ++i) {
        (*address)[i] = 0;
    }

}

void util::init_RatioArray(Ratio** address, int n) {
    (*address) = new util::Ratio[n];
    for (int i = 0; i < n; ++i) {
        (*address)[i] = Ratio(0);
    }
}
void util::init_2dRatioArray(Ratio*** address, int rowNum, int colNum) {
    (*address) = new util::Ratio * [rowNum];
    for (int i = 0; i < rowNum; ++i) {
        (*address)[i] = new util::Ratio[colNum];
        for (int j = 0; j < colNum; ++j) {
            (*address)[i][j] = Ratio(0);
        }
    }
}
void util::init_2dDoubleArray(double*** address, int r, int c) {
    (*address) = new double* [r];
    for (int i = 0; i < r; ++i) {
        (*address)[i] = new double[c];
        for (int j = 0; j < c; j++) {
            (*address)[i][j] = 0;
        }
    }
}
void util::init_2dIntArray(int*** address, int r, int c) {
    (*address) = new int* [r];
    for (int i = 0; i < r; ++i) {
        (*address)[i] = new int[c];
        for (int j = 0; j < c; j++) {
            (*address)[i][j] = 0;
        }
    }
}

vector<vector<double>> util::syncDataFramerate(vector<vector<double>> data, double framerate) {
    double interval = (1.f / framerate);
    vector<vector<double>> out;
    auto it = data.begin();
    int j = 0;
    while (it != data.end()) {
        out.push_back(*it);
        j++;
        it++;
        while (it != data.end() && (*it)[0] <= j * interval) {
            it++;
        }

    }
    return out;
}
// TODO: IMPLEMENTATION
void util::vectorToCSV(vector<vector<double>> vec, string pathToCSV) {

}

pair<double, double> treeMaker(double** map, const double length, const double height) {
    return { 1, 1 };
}
