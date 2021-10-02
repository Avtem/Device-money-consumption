#include <Tests.h>
#include <vector>
#include <av.h>
#include <string>
#include <assert.h>

void UnitTest::runAllTests() // STATIC METHOD
{
    std::vector<bool>resultBools;
    
    assert(getExePath());
}

bool UnitTest::getExePath() // STATIC METHOD
{
    std::wstring expected = L"V:\\0010\\activeProjects\\Visual Studio\\Device money consumption"
                            L"\\#bin\\Win32\\Debug\\Device money consumption.exe";
    
    std::wstring actual =  av::getExePath();
    return expected == actual;
}
