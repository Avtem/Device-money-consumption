#include <LoadSave.h>

#include <windows.h>
#include "../res/rc files/resource.h"
#include <EditThings.h>
#include <assert.h>
#include <string> // for stoi
#include "V:/0010/activeProjects/Visual Studio/_Avlibs/Currency Names/Currency Name Strings.h"

extern std::wstring companyName;
extern std::wstring appName;
extern std::wstring productVersion;
extern std::wstring releaseDate;
extern HWND comboCurrencies;
extern void setEditFloat(int editID, float number, int precision);
extern float getEditFloat(uint editID); 
extern void setCurrency(const wchar_t *countryISO);
extern void setOperandEditValues(int, int, int, float, int daysInUse);
extern void setDefaultCurrency();
static AvIni ini(L"settings.ini");


void loadAllSettings(HWND hwnd, AvTrayIcon &trayIcon)
{
    loadFields();
    loadCurrency();
    loadWindowPlacement(hwnd, trayIcon);
}

void loadWindowPlacement(HWND hwnd, AvTrayIcon &trayIcon)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);  
    if(!ini.loadBinary(L"windowPlacement", &wp, sizeof(wp)))
    {
        ShowWindow(hwnd, SW_SHOW);  // because it's never shown upon creation
        return;  
    }
    
    // This block (BLOCK1) ensures that user will always be able to move the main window. 
    // Imagine what happens if window was positioned in other monitor which
    // no longer accessible while loading (window will be nowhere and there is no way
    // user will be able to change this)
        RECT *wndRect = &wp.rcNormalPosition;
        const int btnTitleCount = 5;
        const int horzOffset = GetSystemMetrics(SM_CXSIZE) *btnTitleCount;
        POINT ptLeftTop =  { wndRect->left, wndRect->top};
        POINT ptRightTop = { wndRect->right -horzOffset, wndRect->top};
    
        HMONITOR currMonitorPtL = MonitorFromPoint(ptLeftTop,  MONITOR_DEFAULTTONULL);
        HMONITOR currMonitorPtR = MonitorFromPoint(ptRightTop, MONITOR_DEFAULTTONULL);
        if(!currMonitorPtL && !currMonitorPtR)
        {
            // returning will cause main window display in the center of primary monitor
            return;
        }
    // end of BLOCK1
        
    bool isTopmost = ini.loadBoolean(L"isTopmost", L"false");
    if(isTopmost)
        av::toggleTopMostStyle(hwnd);

    bool isHiddenInTray = ini.loadBoolean(L"isHiddenInTray", "false");
    if(isHiddenInTray) {
        wp.showCmd = SW_HIDE;
        trayIcon.show();
    }

    SetWindowPlacement(hwnd, &wp);
}

void loadFields()
{
    float number = 0;
    std::wstring str;

    str = ini.loadString(L"wattsUsage", L"5");
    setEditFloat(IDC_EDIT_WATT_USAGE, std::stoi(str), 0);

    str = ini.loadString(L"hours", L"8");
    setEditFloat(IDC_EDIT_HOURS, std::stoi(str), 0);

    str = ini.loadString(L"minutes", L"0");
    setEditFloat(IDC_EDIT_MINUTES, std::stoi(str), 0);

    str = ini.loadString(L"price", L"1.68");
    setEditText(IDC_EDIT_PRICE, str.c_str());

    str = ini.loadString(L"daysInUse", L"30");
    setEditFloat(IDC_EDIT_DAYS_IN_USE, std::stoi(str), 0);
}

void loadCurrency()
{
    std::wstring str = ini.loadString(L"countryISO", L"");
    
    if(str.empty())
    {
        // get current user locale
        GEOID geoID = GetUserGeoID(GEOCLASS_NATION);
        if(geoID == GEOID_NOT_AVAILABLE)
            geoID = 244; // United States
        wchar_t isoCode[3];
        GetGeoInfoW(geoID, GEO_ISO2, isoCode, 3, 0);
        str = isoCode;
    }

    setCurrency(str.c_str());
}


void saveAllSettings(HWND hwnd, const AvTrayIcon &trayIcon)
{
    ini.saveBoolean(L"isTopmost", av::isTopMost(hwnd));

    saveWindowPlacement(hwnd, trayIcon);
    saveFields();
}

void saveWindowPlacement(HWND hwnd, const AvTrayIcon &trayIcon)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    GetWindowPlacement(hwnd, &wp);
    ini.saveBinary(L"windowPlacement", &wp, sizeof(wp));

    ini.saveBoolean(L"isHiddenInTray", trayIcon.isVisible());
}

void saveFields()
{
    ini.saveString(L"wattsUsage", getEditText(IDC_EDIT_WATT_USAGE).c_str());
    ini.saveString(L"hours"     , getEditText(IDC_EDIT_HOURS     ).c_str());
    ini.saveString(L"minutes"   , getEditText(IDC_EDIT_MINUTES   ).c_str());
    ini.saveString(L"price"     , getEditText(IDC_EDIT_PRICE     ).c_str());
    ini.saveString(L"daysInUse" , getEditText(IDC_EDIT_DAYS_IN_USE).c_str());
    
    int index = SendMessage(comboCurrencies, CB_GETCURSEL, 0, 0);
    if(index > 0 && index < currencies.size()) {
        const wchar_t *isoCode = currencies[index].ISO639code;
        ini.saveString(L"countryISO", isoCode);     // avTODO: maybe dangerous?
    }
    else
        assert("Current index in combobox is weird!" == 0);
}