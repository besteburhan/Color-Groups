#ifndef COLORGROUPS_H
#define COLORGROUPS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <Windows.h>

class  ColorGroups {
private:
	cv::Mat image;
	cv::Mat blackBackground;
	cv::Mat black;
	cv::Mat blue;
	cv::Mat pink;
	cv::Mat yellow;
	cv::Mat green;
	cv::Mat orange;
	cv::Mat red;
	cv::Mat unknowns;
	int groupCount=0;
	size_t blackCount = 0;
	size_t blueCount = 0;
	size_t pinkCount = 0;
	size_t yellowCount = 0;
	size_t greenCount = 0;
	size_t orangeCount = 0;
	size_t redCount = 0;
	int width;
	int height;

	std::vector<cv::Vec3b> unknownPixelVect;
	std::vector<size_t> unknownsCounts;
	int unk[20];

	size_t findCont(cv::Mat colors);
public:
	ColorGroups(uchar*  bitmap, int w, int h);
	ColorGroups(const wchar_t* srcImgPath);

	~ColorGroups() {};
	int getGroupCount();
	int getWidth();
	int getHeight();

	int getBlackCount();
	int getBlueCount();
	int getRedCount();
	int getGreenCount();
	int getOrangeCount();
	int getYellowCount();
	int getPinkCount();
	int* getUnknownCount();

	uchar* showBlackColorGroups();
	uchar* showBlueColorGroups();
	uchar* showRedColorGroups();
	uchar* showGreenColorGroups();
	uchar* showOrangeColorGroups();
	uchar* showYellowColorGroups();
	uchar* showPinkColorGroups();
	uchar* showUnknownColorGroups();


};

#endif 

