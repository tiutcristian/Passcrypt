#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <string>

using std::string;

struct char_options
{
    bool uppercase;
    bool lowercase;
    bool numbers;
    bool symbols;
};

void encrypt(const string & s, const string & master, const string & fileName);
string decrypt(const string & fileName, const string & master);
string generatePassword(int len, char_options options);

class decryptionError{};

#endif // ENCRYPT_H
