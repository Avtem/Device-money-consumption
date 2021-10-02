#include <EditThings.h> 
#include "..\res\rc files\resource.h"

extern HWND mainWnd;

std::string getEditText(int editID)
{
    char buffer[20];
    GetDlgItemTextA(mainWnd, editID, buffer, 20);
    return buffer;
}

void setEditFloat(int editID, float number, int precision)
{
    std::string formatStr = "%f";
    if(precision >= 0 && precision < 10)
        formatStr = { '%', '.', (char)('0' +precision), 'f' };
    
    char buffer[20];
    sprintf_s(buffer, formatStr.c_str(), number);
    
    setEditText(editID, buffer);
}

float getEditFloat(uint editID) // tries to convert edits' text to float. Returns 0 if fails
{
    wchar_t editText[20];
    GetDlgItemTextW(mainWnd, editID, editText, 20);
    
    return _wtof(editText);
}

void setEditText(uint editID, const char *text) // sets edit text with restoring user selection
{
    // set text & restore caret position
    int caretStart, caretEnd;
    SendMessage(GetDlgItem(mainWnd, editID), EM_GETSEL, (WPARAM)&caretStart, (LPARAM)&caretEnd);
    
    SetDlgItemTextA(mainWnd, editID, text);
    
    SendMessage(GetDlgItem(mainWnd, editID), EM_SETSEL, caretStart, caretEnd);
}

bool editHasValidValue(uint editID)
{
    if(editID == IDC_EDIT_HOURS)
        return (getEditFloat(editID) >= 0 && getEditFloat(editID) <= 24);
    if(editID == IDC_EDIT_MINUTES)
        return (getEditFloat(editID) >= 0 && getEditFloat(editID) <= 59);

    return false;
}

void setOperandEditValues(int watts, int hours, int minutes, float price)
{
    SetDlgItemInt(mainWnd, IDC_EDIT_WATT_USAGE, watts, false);
    SetDlgItemInt(mainWnd, IDC_EDIT_HOURS     , hours, false);
    SetDlgItemInt(mainWnd, IDC_EDIT_MINUTES   , minutes, false);
    setEditFloat(IDC_EDIT_PRICE, price, 2);
}


void correctEditValue(int editID)
{
    switch(editID)
    {
        case IDC_EDIT_HOURS:    setEditFloat(editID, 24, 0);    break;
        case IDC_EDIT_MINUTES:  setEditFloat(editID, 59, 0);    break;
    }
}
