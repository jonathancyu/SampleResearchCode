#include "frame.h"
#include "Constants.h"
#include <cmath>
#include <utility>

int wrapAbove(int i, int max) {
	return i % max;
}

int wrapBelow(int i, int min, int max) {
	if (i < min) {
		return i + max;
	}
	return i;
}

/*frame::frame(std::string testData) {
	testData += "GradientBasedTrees/";
	userSamples = csvToVector(testData + "userSamples.csv");
	perFrameNumOfParties = csvToRatioVector(testData + "perFrameNumOfParties.csv");
}*/

void frame::processFrameSample() {
	for (vector<double> viewer : userSamples) {
		if (viewer[1] == 400) {
			numOfTestUsers--;
			continue;
		}
		double theta = viewer[1] + 180;
		double phi = viewer[2] + 90;
		theta *= length / 360;
		phi *= height / 180;
		int thetaTile = floor(theta / tileLength);
		int phiTile = floor(phi / tileHeight);
		//TODO: lots of duplicate code here (even w/ wrap functions)
		int thetaRightTile = wrapAbove(thetaTile + scale, colNum);
		int thetaLeftTile = wrapBelow(thetaTile - scale, 0, colNum);
		int phiUpTile = wrapAbove(phiTile + scale, rowNum);
		int phiDownTile = wrapBelow(phiTile - scale, 0, rowNum);

		thetaRightTile = wrapBelow(thetaRightTile, thetaLeftTile, colNum);
		phiUpTile = wrapBelow(phiUpTile, phiDownTile, rowNum);

		int phiUpTileDown = wrapBelow(phiUpTile - 1, 0, rowNum);
		int phiDownTileUp = wrapAbove(phiDownTile + 1, rowNum);
		int thetaRightTileLeft = wrapBelow(thetaRightTile - 1, 0, colNum);
		int thetaLeftTileRight = wrapAbove(thetaLeftTile + 1, colNum);

		double thetaRightPercent = std::fmod(theta, tileLength) / tileLength;
		double thetaLeftPercent = 1 - thetaRightPercent;
		double phiUpPercent = std::fmod(phi, tileHeight) / tileHeight;
		double phiDownPercent = 1 - phiUpPercent;

		std::pair<int, int> upLeft;
		if (thetaLeftPercent >= 0.5 && phiUpPercent >= 0.5) {
			upLeft = { phiUpTile, thetaLeftTile };
		} else if (thetaLeftPercent >= 0.5) {
			upLeft = { phiUpTileDown, thetaLeftTile };
		} else if (phiUpPercent >= 0.5) {
			upLeft = { phiUpTile, thetaLeftTileRight };
		} else {
			upLeft = { phiUpTileDown, thetaLeftTileRight };
		}
		std::pair<int, int> downRight;
		if (thetaRightPercent >= 0.5 && phiDownPercent >= 0.5) {
			downRight = { phiDownTile, thetaRightTile };
		} else if (thetaRightPercent >= 0.5) {
			downRight = { phiDownTileUp, thetaRightTile };
		} else if (phiDownPercent >= 0.5) {
			downRight = { phiDownTile, thetaRightTileLeft };
		} else {
			downRight = { phiDownTileUp, thetaRightTileLeft };
		}
		if (upLeft.first < downRight.first) {
			upLeft.first = upLeft.first + rowNum;
		}
		if (upLeft.second > downRight.second) {
			downRight.second = downRight.second + colNum;
		}

		std::pair<std::pair<int, int>, std::pair<int, int>> viewportBounds = { upLeft, downRight };
		int** tilesInActualVP;
		init_2dIntArray(&tilesInActualVP, 2, VPlength * VPheight);
		int col = 0;
		for (int i = downRight.first; i < upLeft.first; i++) {
			for (int j = upLeft.second; j < downRight.second; j++) {
				tilesInActualVP[1][col] = i;
				tilesInActualVP[2][col] = j;
				col++;
			}
		}
		/*
		for (Ratio viewport : perFrameNumOfParties) {

		}*/

		//line 303

		
	}
}

