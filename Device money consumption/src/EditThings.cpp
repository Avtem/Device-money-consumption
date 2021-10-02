#include <EditThings.h> 
#include "..\res\rc files\resource.h"

extern HWND mainWnd;

std::wstring getEditText(int editID)
{
    wchar_t buffer[20];
    GetDlgItemTextW(mainWnd, editID, buffer, 20);
    return buffer;
}

void setEditFloat(int editID, float number, int precision)
{
    std::wstring formatStr = L"%f";
    if(precision >= 0 && precision < 10)
        formatStr = { '%', '.', (wchar_t)('0' +precision), 'f' };
    
    wchar_t buffer[20];
    swprintf_s(buffer, 20, formatStr.c_str(), number);
    
    setEditText(editID, buffer);
}

float getEditFloat(uint editID) // tries to convert edits' text to float. Returns 0 if fails
{
    wchar_t editText[20];
    GetDlgItemTextW(mainWnd, editID, editText, 20);
    
    return _wtof(editText);
}

void setEditText(uint editID, cwstr text) // sets edit text with restoring user selection
{
    // set text & restore caret position
    int caretStart, caretEnd;
    SendMessage(GetDlgItem(mainWnd, editID), EM_GETSEL, (WPARAM)&caretStart, (LPARAM)&caretEnd);
    
    SetDlgItemTextW(mainWnd, editID, text);
    
    SendMessage(GetDlgItem(mainWnd, editID), EM_SETSEL, caretStart, caretEnd);
}

bool editHasValidValue(uint editID)
{
    if(editID == IDC_EDIT_HOURS)
        return (getEditFloat(editID) >= 0 && getEditFloat(editID) <= 24);
    if(editID == IDC_EDIT_MINUTES)
        return (getEditFloat(editID) >= 0 && getEditFloat(editID) <= 59);

    return true;
}

void setOperandEditValues(int watts, int hours, int minutes, float price, int daysInUse)
{
    SetDlgItemInt(mainWnd, IDC_EDIT_WATT_USAGE, watts    , false);
    SetDlgItemInt(mainWnd, IDC_EDIT_HOURS     , hours    , false);
    SetDlgItemInt(mainWnd, IDC_EDIT_MINUTES   , minutes  , false);
    SetDlgItemInt(mainWnd, IDC_EDIT_DAYS_IN_USE    , daysInUse, false);
    setEditFloat(IDC_EDIT_PRICE, price, 2);
}


float getCorrectEditValue(int editID)
{
    switch(editID)
    {
        case IDC_EDIT_HOURS:    return 24.0f;
        case IDC_EDIT_MINUTES:  return 59.0f;
    }
}
