#pragma once

#include <ColorGroups.h>

extern "C" {

	__declspec(dllexport) ColorGroups* CreateColorGroups(const wchar_t* pathName);
	__declspec(dllexport) ColorGroups* CreateColorGroupsWithBitmap(uchar* bitmap, int w, int h);
	__declspec(dllexport) void DeleteColorGroups(ColorGroups* obj);
	__declspec(dllexport) int GetBlackCount(ColorGroups* obj);
	__declspec(dllexport) int GetBlueCount(ColorGroups* obj);
	__declspec(dllexport) int GetYellowCount(ColorGroups* obj);
	__declspec(dllexport) int GetPinkCount(ColorGroups* obj);
	__declspec(dllexport) int GetRedCount(ColorGroups* obj);
	__declspec(dllexport) int GetOrangeCount(ColorGroups* obj);
	__declspec(dllexport) int GetGreenCount(ColorGroups* obj);
	__declspec(dllexport) int GetGroupCount(ColorGroups* obj);
	__declspec(dllexport) int* GetUnknownCount(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowBlackColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowBlueColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowRedColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowGreenColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowOrangeColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowYellowColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowPinkColorGroups(ColorGroups* obj);
	__declspec(dllexport) uchar* ShowUnknownColorGroups(ColorGroups* obj);
	__declspec(dllexport) int GetWidth(ColorGroups* obj);
	__declspec(dllexport) int GetHeight(ColorGroups* obj);
}