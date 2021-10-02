/* This file was created by Avtem using Visual Studio IDE.
 * 
 * Created: 2/8/2021 8:02:38 PM 
 * Version 1.0.0:	dd.mm.yyyy*/

#pragma once

#include <string>
#include <av.h>

bool regKeyExists(const wchar_t * keyPath);
bool regValueExists(const wchar_t * keyPath);
std::wstring getRegKeyPath();

bool loadAllSettings(HWND hwnd, AvTrayIcon &trayIcon);
bool loadWindowPlacement(HWND hwnd, AvTrayIcon &trayIcon);
void loadFields();
bool loadSetting(const wchar_t * valueName, void *data, int dataType = REG_DWORD, uint dataSize = 4);
void saveSetting(const wchar_t *valueName, void *data, int dataType = REG_DWORD, uint dataSize = 4);
void saveFloat(const wchar_t * valueName, float number);
void saveAllSettings(HWND hwnd, const AvTrayIcon &trayIcon);
void saveWindowPlacement(HWND hwnd, const AvTrayIcon &trayIcon);
void saveFields();
void setDefaultSettings(HWND hwnd);
