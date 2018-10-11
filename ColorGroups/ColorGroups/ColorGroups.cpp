#include "ColorGroups.h"
#include <opencv2/opencv.hpp>
#include <iostream>
//#include <sstream>
#include <string>

using std::cout;
using std::endl;
using namespace cv;


ColorGroups::ColorGroups(uchar* bitmap,int w,int h) {

	Mat img(Size(w, h), CV_8UC3,bitmap);
	this->image = img;

	//std::memcpy(this->image.data, (uchar *)bitmap, stride);

	
	
	//image
	if (this->image.empty()) {
		throw std::invalid_argument("There is no image!");
	}
	
	this->width = w;
	this->height = h;

	//black,blackCount
	inRange(this->image, Scalar(0, 0, 0), Scalar(50, 50, 50), this->black);
	dilate(this->black, this->black, Mat::ones(3, 3, CV_8UC1));
	this->blackCount = findCont(this->black);

	//blackBackground
	Mat th;
	cvtColor(this->image, th, COLOR_BGR2GRAY);
	threshold(th, th, 220, 255, THRESH_BINARY_INV);
	bitwise_and(this->image, this->image, this->blackBackground, th);

	//blue,blueCount
	inRange(this->image, Scalar(193, 0, 0), Scalar(255, 185, 115), this->blue);
	dilate(this->blue, this->blue, Mat::ones(3, 3, CV_8UC1));
	this->blueCount = findCont(this->blue);


	//red,redCount
	inRange(this->image, Scalar(0, 0, 196), Scalar(70, 70, 255), this->red);
	dilate(this->red, this->red, Mat::ones(3, 3, CV_8UC1));
	this->redCount = findCont(this->red);

	//green,greenCount
	inRange(this->image, Scalar(0, 130, 0), Scalar(85, 255, 85), this->green);
	dilate(this->green, this->green, Mat::ones(3, 3, CV_8UC1));
	this->greenCount = findCont(this->green);

	//orange,orangeCount
	inRange(this->image, Scalar(0, 95, 235), Scalar(70, 175, 255), this->orange);
	dilate(this->orange, this->orange, Mat::ones(3, 3, CV_8UC1));
	this->orangeCount = findCont(this->orange);

	//yellow,yellowCount
	inRange(this->image, Scalar(0, 200, 230), Scalar(100, 255, 255), this->yellow);
	dilate(this->yellow, this->yellow, Mat::ones(3, 3, CV_8UC1));
	this->yellowCount = findCont(this->yellow);

	//pink,pinkCount
	inRange(this->image, Scalar(200, 0, 230), Scalar(250, 175, 255), this->pink);
	dilate(this->pink, this->pink, Mat::ones(3, 3, CV_8UC1));
	this->pinkCount = findCont(this->pink);

	//creating unknowns image
	std::vector<Mat> mats = { this->blue,this->red,this->green,this->orange,this->yellow,this->pink };
	Mat blackBackgroundCopy = blackBackground.clone();
	for (auto m : mats) {
		Mat change;
		threshold(m, m, 100, 255, THRESH_BINARY_INV);
		bitwise_and(blackBackgroundCopy, blackBackgroundCopy, change, m);
		blackBackgroundCopy = change;
	}
	this->unknowns = blackBackgroundCopy;


	//adding unkown colors to unknownPixelVect
	this->unknownPixelVect.push_back(this->unknowns.at<Vec3b>(0, 0));
	for (size_t i = 0; i < height; i++)
		for (size_t j = 0; j < width; j++) {

			Vec3b px = this->unknowns.at<Vec3b>(i, j);

			for (size_t vInd = 0; vInd < this->unknownPixelVect.size(); vInd++) {
				if (this->unknownPixelVect.at(vInd) == px) break;
				else if (vInd == this->unknownPixelVect.size() - 1) {
					this->unknownPixelVect.push_back(px);
				}
			}
		}


	//when unknown pixels in unknownPixelVect are in +-40 range,delete them
	for (size_t i = 0; i < this->unknownPixelVect.size(); i++)
		for (size_t j = (i + 1); j < this->unknownPixelVect.size(); j++) {
			Vec3b del;
			if (((this->unknownPixelVect[i][0] <= this->unknownPixelVect[j][0] + 40) && (this->unknownPixelVect[i][0] >= this->unknownPixelVect[j][0] - 40)) &&
				((this->unknownPixelVect[i][1] <= this->unknownPixelVect[j][1] + 40) && (this->unknownPixelVect[i][1] >= this->unknownPixelVect[j][1] - 40)) &&
				((this->unknownPixelVect[i][2] <= this->unknownPixelVect[j][2] + 40) && (this->unknownPixelVect[i][2] >= this->unknownPixelVect[j][2] - 40)))
			{
				this->unknownPixelVect.erase(this->unknownPixelVect.begin() + j);
				j--;
			}
		}


	// counting colors wrt unknownPixelVect
	for (size_t i = 0; i < this->unknownPixelVect.size(); i++) {
		if (this->unknownPixelVect.at(i) != Vec3b(0, 0, 0)) {
			Vec3b unknownPx = this->unknownPixelVect.at(i);
			Mat others;
			inRange(this->image, Scalar(unknownPx[0] - 40, unknownPx[1] - 40, unknownPx[2] - 40), Scalar(unknownPx[0] + 40, unknownPx[1] + 40, unknownPx[2] + 40), others);
			dilate(others, others, Mat::ones(3, 3, CV_8UC1));
			size_t unknownCount = findCont(others);
			this->unknownsCounts.push_back(unknownCount);
		}
	}


	//groupCount update
	this->groupCount += this->unknownsCounts.size() + (this->blackCount == 0 ? 0 : 1) + (this->blueCount == 0 ? 0 : 1) + (this->redCount == 0 ? 0 : 1) + (this->yellowCount == 0 ? 0 : 1) +
		(this->greenCount == 0 ? 0 : 1) + (this->orangeCount == 0 ? 0 : 1) + (this->pinkCount == 0 ? 0 : 1);

	//update colors
	Mat temp;
	threshold(blue, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, blue, temp);
	
	threshold(red, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, red, temp);
	

	threshold(green, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, green, temp);

	threshold(orange, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, orange, temp);

	threshold(yellow, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, yellow, temp);

	threshold(pink, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, pink, temp);

	threshold(black, temp, 60, 255, THRESH_BINARY);
	bitwise_and(this->blackBackground, this->blackBackground, black, temp);
	temp.copyTo(this->black);

	

}



//string constructor
ColorGroups::ColorGroups(const wchar_t* srcImgPath) {

	//wchar_t -> string
	std::ostringstream stm;
	while (*srcImgPath != L'\0') {
		stm << std::use_facet< std::ctype<wchar_t> >(std::locale()).narrow(*srcImgPath++, '?');
	}

	//image
	this->image = imread(stm.str());
	if (this->image.empty()) {
		throw std::invalid_argument("There is no image!");
	}

	this->width = image.size().width;
	this->height = image.size().height;

	//black,blackCount
	inRange(this->image, Scalar(0, 0, 0), Scalar(60, 60, 60), this->black);
	dilate(this->black, this->black, Mat::ones(3, 3, CV_8UC1));
	this->blackCount = findCont(this->black);

	//blackBackground
	Mat th;
	cvtColor(this->image, th, COLOR_BGR2GRAY);
	threshold(th, th, 220, 255, THRESH_BINARY_INV);
	bitwise_and(this->image, this->image, this->blackBackground, th);

	//blue,blueCount
	inRange(this->image, Scalar(193, 0, 0), Scalar(255, 185, 115), this->blue);
	dilate(this->blue, this->blue, Mat::ones(3, 3, CV_8UC1));
	this->blueCount = findCont(this->blue);
	

	//red,redCount
	inRange(this->image, Scalar(0, 0, 196), Scalar(70, 70, 255), this->red);
	dilate(this->red, this->red, Mat::ones(3, 3, CV_8UC1));
	this->redCount = findCont(this->red);

	//green,greenCount
	inRange(this->image, Scalar(0, 130, 0), Scalar(85, 255, 85), this->green);
	dilate(this->green, this->green, Mat::ones(3, 3, CV_8UC1));
	this->greenCount = findCont(this->green);

	//orange,orangeCount
	inRange(this->image, Scalar(0, 95, 235), Scalar(70, 175, 255), this->orange);
	dilate(this->orange, this->orange, Mat::ones(3, 3, CV_8UC1));
	this->orangeCount = findCont(this->orange);

	//yellow,yellowCount
	inRange(this->image, Scalar(0, 200, 230), Scalar(100, 255, 255), this->yellow);
	dilate(this->yellow, this->yellow, Mat::ones(3, 3, CV_8UC1));
	this->yellowCount = findCont(this->yellow);

	//pink,pinkCount
	inRange(this->image, Scalar(200, 0, 230), Scalar(250, 175, 255), this->pink);
	dilate(this->pink, this->pink, Mat::ones(3, 3, CV_8UC1));
	this->pinkCount = findCont(this->pink);

	//creating unknowns image
	std::vector<Mat> mats = { this->blue,this->red,this->green,this->orange,this->yellow,this->pink };
	Mat blackBackgroundCopy = blackBackground.clone();
	for (auto m : mats) {
		Mat change;
		threshold(m, m, 100, 255, THRESH_BINARY_INV);
		bitwise_and(blackBackgroundCopy, blackBackgroundCopy, change, m);
		blackBackgroundCopy = change;
	}
	this->unknowns = blackBackgroundCopy;


	//adding unkown colors to unknownPixelVect
	this->unknownPixelVect.push_back(this->unknowns.at<Vec3b>(0, 0));
	for (size_t i = 0; i < height; i++)
		for (size_t j = 0; j < width; j++) {

			Vec3b px = this->unknowns.at<Vec3b>(i, j);

			for (size_t vInd = 0; vInd < this->unknownPixelVect.size(); vInd++) {
				if (this->unknownPixelVect.at(vInd) == px) break;
				else if (vInd == this->unknownPixelVect.size() - 1) {
					this->unknownPixelVect.push_back(px);
				}
			}
		}


	//when unknown pixels in unknownPixelVect are in +-40 range,delete them
	for (size_t i = 0; i < this->unknownPixelVect.size(); i++)
		for (size_t j = (i + 1); j < this->unknownPixelVect.size(); j++) {
			Vec3b del;
			if (((this->unknownPixelVect[i][0] <= this->unknownPixelVect[j][0] + 40) && (this->unknownPixelVect[i][0] >= this->unknownPixelVect[j][0] - 40)) &&
				((this->unknownPixelVect[i][1] <= this->unknownPixelVect[j][1] + 40) && (this->unknownPixelVect[i][1] >= this->unknownPixelVect[j][1] - 40)) &&
				((this->unknownPixelVect[i][2] <= this->unknownPixelVect[j][2] + 40) && (this->unknownPixelVect[i][2] >= this->unknownPixelVect[j][2] - 40)))
			{
				this->unknownPixelVect.erase(this->unknownPixelVect.begin() + j);
				j--;
			}
		}


	// counting colors wrt unknownPixelVect
	for (size_t i = 0; i < this->unknownPixelVect.size(); i++) {
		if (this->unknownPixelVect.at(i) != Vec3b(0, 0, 0)) {
			Vec3b unknownPx = this->unknownPixelVect.at(i);
			Mat others;
			inRange(this->image, Scalar(unknownPx[0] - 40, unknownPx[1] - 40, unknownPx[2] - 40), Scalar(unknownPx[0] + 40, unknownPx[1] + 40, unknownPx[2] + 40), others);
			dilate(others, others, Mat::ones(3, 3, CV_8UC1));
			size_t unknownCount = findCont(others);
			this->unknownsCounts.push_back(unknownCount);
		}
	}


	//groupCount update
	this->groupCount += this->unknownsCounts.size() + (this->blackCount == 0 ? 0 : 1) + (this->blueCount == 0 ? 0 : 1) + (this->redCount == 0 ? 0 : 1) + (this->yellowCount == 0 ? 0 : 1) +
		(this->greenCount == 0 ? 0 : 1) + (this->orangeCount == 0 ? 0 : 1) + (this->pinkCount == 0 ? 0 : 1);

	//update colors
	Mat temp;
	threshold(blue, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, blue, temp);

	threshold(red, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, red, temp);

	threshold(green, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, green, temp);

	threshold(orange, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, orange, temp);

	threshold(yellow, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, yellow, temp);

	threshold(pink, temp, 100, 255, THRESH_BINARY_INV);
	bitwise_and(this->blackBackground, this->blackBackground, pink, temp);

	threshold(black, temp, 60, 255, THRESH_BINARY);
	bitwise_and(this->blackBackground, this->blackBackground, black, temp);
	temp.copyTo(this->black);

}


int ColorGroups::getWidth() {
	return this->width;
}
int ColorGroups::getHeight() {
	return this->height;

}

size_t ColorGroups::findCont(Mat colors) {
	std::vector<std::vector<Point> > contours;
	Mat cOutput = colors.clone();
	findContours(cOutput, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	return contours.size();
}



int ColorGroups::getGroupCount() {
	return this->groupCount;
}
int ColorGroups::getBlackCount() {
	return this->blackCount;
}
int ColorGroups::getBlueCount() {
	return this->blueCount;
}
int ColorGroups::getRedCount() {
	return this->redCount;
}
int ColorGroups::getGreenCount() {
	return this->greenCount;
}
int ColorGroups::getOrangeCount() {
	return this->orangeCount;
}
int ColorGroups::getYellowCount() {
	return this->yellowCount;
}
int ColorGroups::getPinkCount() {
	return this->pinkCount;
}
int* ColorGroups::getUnknownCount() {

	int i;
	for (i = 0; i < unknownsCounts.size(); i++) {
		unk[i] = unknownsCounts.at(i);
	}
	unk[i] = -1;
	return unk;
}




uchar* ColorGroups::showBlackColorGroups() {
	return black.data;
}
uchar* ColorGroups::showBlueColorGroups() {
	return this->blue.data;
}
uchar* ColorGroups::showRedColorGroups() {
	
	return red.data;
}
uchar* ColorGroups::showGreenColorGroups() {
	return green.data;
}
uchar* ColorGroups::showOrangeColorGroups() {
	return orange.data;
}
uchar* ColorGroups::showYellowColorGroups() {
	return yellow.data;
}
uchar* ColorGroups::showPinkColorGroups() {
	Mat temp;
	return pink.data;
}
uchar* ColorGroups::showUnknownColorGroups() {
	return unknowns.data;
}



