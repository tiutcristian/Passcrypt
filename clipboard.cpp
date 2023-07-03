#include "clipboard.h"
#include <windows.h>

void copyToClipboard(std::string s)
{
    if (!OpenClipboard(NULL))
    {
        // something went wrong
        return;
    }

    // Remove the current Clipboard contents
    if (!EmptyClipboard())
    {
        // something went wrong
        return;
    }

    UINT clipboardHistoryFormat_ = RegisterClipboardFormat(L"CanIncludeInClipboardHistory");
    UINT cloudClipboardFormat_ = RegisterClipboardFormat(L"CanUploadToCloudClipboard");

    auto number_of_bytes = sizeof(DWORD);
    auto hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, number_of_bytes);
    if (SetClipboardData(clipboardHistoryFormat_, hMem) == nullptr)
    {
        // something went wrong
        return;
    }
    if (SetClipboardData(clipboardHistoryFormat_, hMem) == nullptr)
    {
        // something went wrong
        return;
    }

    // Get the currently selected data
    HGLOBAL hGlob = GlobalAlloc(GMEM_FIXED, s.length()+1);
    strcpy_s((char*)hGlob, s.length()+1, s.c_str());

    if (::SetClipboardData(CF_TEXT, hGlob) == NULL)
    {
        // something went wrong
        return;
    }

    CloseClipboard();
}

void clearClipboard()
{
    if (!OpenClipboard(NULL))
    {
        // something went wrong
        return;
    }

    // Remove the current Clipboard contents
    if (!EmptyClipboard())
    {
        // something went wrong
        return;
    }

    CloseClipboard();
}

bool checkClipboardContent(std::string s)
{
    if (!OpenClipboard(NULL))
    {
        // something went wrong
        return false;
    }

    auto handle = GetClipboardData(CF_TEXT);
    if(!handle)
        return false;
    char* s2 = new char[s.length()+1];
    strcpy_s(s2, s.length()+1, (char*)handle);
    int rezultat = strcmp(s.c_str(), s2);
    delete[] s2;
    CloseClipboard();
    return rezultat == 0;
}
