#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <string>

void copyToClipboard(std::string s);
void clearClipboard();
bool checkClipboardContent(std::string s);

#endif // CLIPBOARD_H
