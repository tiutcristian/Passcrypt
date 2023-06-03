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

string encrypt(const string & s, const string & master);
string decrypt(const string & s, const string & master);
string generatePassword(int len, char_options options);

#endif // ENCRYPT_H
