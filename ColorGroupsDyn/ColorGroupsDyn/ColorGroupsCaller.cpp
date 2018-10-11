#include "ColorGroupsCaller.h"
#include <windows.h>


ColorGroups* CreateColorGroupsWithBitmap(uchar* bitmap, int w, int h) {
	ColorGroups* obj = new ColorGroups(bitmap, w, h);
	return obj;
}
ColorGroups* CreateColorGroups(const wchar_t* pathName) {
	ColorGroups* obj = new ColorGroups(pathName);
	return obj;
}

void DeleteColorGroups(ColorGroups* obj) {
	if (obj != NULL)
	{
		delete obj;
		obj = NULL;
	}
}
int GetBlackCount(ColorGroups* obj) {
	return obj->getBlackCount();
}
int GetBlueCount(ColorGroups* obj) {
	return obj->getBlueCount();
}
int GetYellowCount(ColorGroups* obj) {
	return obj->getYellowCount();
}
int GetPinkCount(ColorGroups* obj) {
	return obj->getPinkCount();
}
int GetRedCount(ColorGroups* obj) {
	return obj->getRedCount();
}
int GetOrangeCount(ColorGroups* obj) {
	return obj->getOrangeCount();
}
int GetGreenCount(ColorGroups* obj) {
	return obj->getGreenCount();
}
int GetGroupCount(ColorGroups* obj) {
	return obj->getGroupCount();
}
int* GetUnknownCount(ColorGroups* obj) {
	return obj->getUnknownCount();
}

uchar* ShowBlackColorGroups(ColorGroups* obj) {
	return obj->showBlackColorGroups();
}
uchar* ShowBlueColorGroups(ColorGroups* obj) {
	return obj->showBlueColorGroups();
}
uchar* ShowRedColorGroups(ColorGroups* obj) {
	return obj->showRedColorGroups();
}
uchar* ShowGreenColorGroups(ColorGroups* obj) {
	return obj->showGreenColorGroups();
}
uchar* ShowOrangeColorGroups(ColorGroups* obj) {
	return obj->showOrangeColorGroups();
}
uchar* ShowYellowColorGroups(ColorGroups* obj) {
	return obj->showYellowColorGroups();
}
uchar* ShowPinkColorGroups(ColorGroups* obj) {
	return obj->showPinkColorGroups();
}
uchar* ShowUnknownColorGroups(ColorGroups* obj) {
	return obj->showUnknownColorGroups();
}
int GetWidth(ColorGroups* obj) {
	return obj->getWidth();
}
int GetHeight(ColorGroups* obj) {
	return obj->getHeight();
}

