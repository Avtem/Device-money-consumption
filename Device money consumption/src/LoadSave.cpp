#include <LoadSave.h>

#include <windows.h>
#include "../res/rc files/resource.h"
#include <EditThings.h>
#include <assert.h>
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

void saveFloat(const wchar_t * valueName, float number)
{
    float f = number;
    saveSetting(valueName, &f);
}

std::wstring getRegKeyPath()
{
    return std::wstring(L"SOFTWARE\\") + companyName 
                 + std::wstring(L"\\") + appName;
}

bool regKeyExists(const wchar_t * keyPath)
{
    HKEY hKey;
    bool regKeyExists = ERROR_SUCCESS == RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, 
                                                       KEY_READ, &hKey);
    RegCloseKey(hKey);
    
    return regKeyExists;
}

bool regValueExists(const wchar_t *value)
{
    HKEY hKey;
    std::wstring keyPath = getRegKeyPath();
    bool regKeyExists = ERROR_SUCCESS == RegOpenKeyExW(HKEY_CURRENT_USER, keyPath.c_str(),
                                                       0, KEY_READ, &hKey);
    if(!regKeyExists)
        return false;

    LONG result = RegQueryValueExW(hKey, value, 0, 0, 0, 0);
    RegCloseKey(hKey);

    return result == ERROR_SUCCESS;
}
 
bool loadAllSettings(HWND hwnd, AvTrayIcon &trayIcon)
{
    std::wstring regPath = getRegKeyPath();
    if(!regKeyExists( regPath.c_str() ) ) 
        return false;

    loadFields();
    if(!loadWindowPlacement(hwnd, trayIcon))
        return false;

    BOOL isTopmost = false;
    loadSetting(L"isTopmost", &isTopmost);
    if(isTopmost)
        av::toggleTopMostStyle(hwnd);

    return true;
}

bool loadWindowPlacement(HWND hwnd, AvTrayIcon &trayIcon)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);  
    if(!loadSetting(L"windowPlacement", &wp, REG_BINARY, sizeof(wp)))
        return false;
    
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
            return false;
        }
    // end of BLOCK1

    BOOL isMinimized = false;
    loadSetting(L"isMinimized", &isMinimized);
    if(isMinimized) {
        wp.showCmd = SW_HIDE;
        trayIcon.show();
    }

    SetWindowPlacement(hwnd, &wp);
    return true;
}

void loadFields()
{
    float number = 0;
    loadSetting(L"wattsUsage", &number);
    setEditFloat(IDC_EDIT_WATT_USAGE, number, 0);

    loadSetting(L"hours", &number);
    setEditFloat(IDC_EDIT_HOURS, number, 0);

    loadSetting(L"minutes", &number);
    setEditFloat(IDC_EDIT_MINUTES, number, 0);

    loadSetting(L"price", &number);
    setEditFloat(IDC_EDIT_PRICE, number, 2);

    loadSetting(L"daysInUse", &number);
    setEditFloat(IDC_DAYS_IN_USE, number, 0);

    wchar_t countryISO[3] = L"US";
    loadSetting(L"countryISO", &countryISO, REG_BINARY, 6);
    setCurrency(countryISO);
}

bool loadSetting(const wchar_t * valueName, void *data, int dataType, uint dataSize)
{
    std::wstring regPath = getRegKeyPath();
    if(!regValueExists(valueName))
        return false;

    uint restrictType = dataType == REG_DWORD ? RRF_RT_DWORD : RRF_RT_REG_BINARY;
    RegGetValueW(HKEY_CURRENT_USER, regPath.c_str(), valueName, restrictType,
                 (DWORD *)(&dataType), data, (DWORD *)(&dataSize));
    return true;
}

void saveAllSettings(HWND hwnd, const AvTrayIcon &trayIcon)
{
    BOOL isTopmost = av::isTopMost(hwnd);
    saveSetting(L"isTopmost", &isTopmost);

    saveWindowPlacement(hwnd, trayIcon);
    saveFields();
}

void saveWindowPlacement(HWND hwnd, const AvTrayIcon &trayIcon)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    GetWindowPlacement(hwnd, &wp);
    saveSetting(L"windowPlacement", &wp, REG_BINARY, sizeof(wp));

    BOOL isMinimized = trayIcon.isVisible();
    saveSetting(L"isMinimized", &isMinimized);
}

void saveFields()
{
    saveFloat(L"wattsUsage", getEditFloat(IDC_EDIT_WATT_USAGE));
    saveFloat(L"hours", getEditFloat(IDC_EDIT_HOURS));
    saveFloat(L"minutes", getEditFloat(IDC_EDIT_MINUTES));
    saveFloat(L"price", getEditFloat(IDC_EDIT_PRICE));
    saveFloat(L"daysInUse", getEditFloat(IDC_DAYS_IN_USE));
    
    int index = SendMessage(comboCurrencies, CB_GETCURSEL, 0, 0);
    if(index > 0 && index < currencies.size()) {
        const wchar_t *isoCode = currencies[index].ISO639code;
        saveSetting(L"countryISO", (void*)isoCode, REG_BINARY, 3 *2);
    }
    else
        assert("Current index in combobox is weird!" == 0);
}

void setDefaultSettings(HWND hwnd)
{
    setDefaultCurrency();
    setOperandEditValues(5, 8, 0, 1.75, 30);
    ShowWindow(hwnd, SW_SHOW);
}

void saveSetting(const wchar_t *valueName, void *data, int dataType, uint dataSize)
{ 
    HKEY hKey; 
    std::wstring regPath = getRegKeyPath();
    RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, 
                    KEY_CREATE_SUB_KEY | KEY_SET_VALUE, 0, &hKey, nullptr);

    int res = RegSetValueExW(hKey, valueName, 0, dataType, 
                                       (const byte *) data, dataSize);
#ifdef _DEBUG
    assert(ERROR_SUCCESS == res);
#endif
    
    RegCloseKey(hKey);
}