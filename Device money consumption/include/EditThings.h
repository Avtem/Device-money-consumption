/* This file was created by Avtem using Visual Studio IDE.
 * 
 * Created: 2/8/2021 8:56:47 PM 
 * Version 1.0.0:	dd.mm.yyyy*/

#pragma once
#include <string>
#include <av.h>

std::wstring getEditText(int editID);
void setEditText(uint editID, cwstr text); // sets edit text with restoring user selection
float getEditFloat(uint editID); // tries to convert edits' text to float. Returns 0 if fails
void setEditFloat(int editID, float number, int precision = -1);
bool editHasValidValue(uint editID);
void setOperandEditValues(int watts, int hours, int minutes, float price, int daysInUse);
float getCorrectEditValue(int editID);