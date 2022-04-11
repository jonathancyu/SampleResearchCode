#pragma once
#include <string>
#include <vector>

#include "util.h"
using namespace util;


class frame {
	//all temp variables so we can implement quickly
	std::vector<std::vector<double>> userSamples;
	int frameNumber;
	int frameCount;
	int numOfTestUsers;
	double length;
	double height;
	double tileLength;
	double tileHeight;
	vector<Ratio> perFrameNumOfParties;

	frame() = default;
	//frame(std::string testData);

	void processFrameSample();
};

