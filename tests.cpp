#include "tests.h"
#include "util.h"
#include "Video.h"
#include <fstream>

std::string testPath = "C:/Users/yucjo/Documents/2021 summer/DSLab/360DegreeVideo/TestData/";


bool readingTrainingDataTest() {
    Video v1 = Video("C:/Users/yucjo/Documents/2021 summer/DSLab/Videos/original/", 1);
    v1.process("C:/Users/yucjo/Documents/2021 summer/DSLab/Traces/VideoIDprocessed/");
	return false;
}



bool BucketizeTest() {
	double tolerance = 4.5e-8;
	auto expected = util::csvToVector(testPath + "BucketizeTestOut.csv");
	auto input = util::csvToVector(testPath + "BucketizeTestIn.csv");
	auto output = util::syncDataFramerate(input, 25.0);
	int result = tests::compare2dVector(expected, output, tolerance);
	return  result == -1;
}

tests::tests() {
	items.emplace_back("BucketizeTest", BucketizeTest);
	items.emplace_back("readingTrainingDataTest", readingTrainingDataTest);
}

