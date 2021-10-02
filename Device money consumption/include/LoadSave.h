/* This file was created by Avtem using Visual Studio IDE.
 * 
 * Created: 2/8/2021 8:02:38 PM 
 * Version 1.0.0:	dd.mm.yyyy*/

#pragma once

#include <string>
#include <av.h>

void loadAllSettings(HWND hwnd, AvTrayIcon &trayIcon);
void loadWindowPlacement(HWND hwnd, AvTrayIcon &trayIcon);
void loadFields();
void loadCurrency();

void saveAllSettings(HWND hwnd, const AvTrayIcon &trayIcon);
void saveWindowPlacement(HWND hwnd, const AvTrayIcon &trayIcon);
void saveFields();