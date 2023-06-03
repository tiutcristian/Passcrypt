/*#include "encrypt.h"
#include <bits/stdc++.h>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cryptopp/default.h>
using CryptoPP::DefaultEncryptorWithMAC;
using CryptoPP::DefaultDecryptorWithMAC;

#include <cryptopp/filters.h>
using CryptoPP::StringSource;
using CryptoPP::StringSink;

#include <cryptopp/files.h>
using CryptoPP::FileSource;

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

using CryptoPP::byte;
using std::vector;

using std::cin;
using std::cout;

int lenmax;
string s;
vector<string> key;
char_options cho;

void read(string & s, char_options & cho, int & lenmax)
{
    char aux;
    cout << "Enter password to encrypt: ";                          cin >> s;
    cout << "Should the password have uppercase letters? [Y/N] : "; cin >> aux; cho.uppercase = ( aux == 'y' || aux == 'Y' );
    cout << "Should the password have lowercase letters? [Y/N] : "; cin >> aux; cho.lowercase = ( aux == 'y' || aux == 'Y' );
    cout << "Should the password have numbers? [Y/N] : ";           cin >> aux; cho.numbers   = ( aux == 'y' || aux == 'Y' );
    cout << "Should the password have symbols? [Y/N] : ";           cin >> aux; cho.symbols   = ( aux == 'y' || aux == 'Y' );
    cout << "Enter the maximum number of characters: ";             cin >> lenmax;
}

void create_sym_vector(vector<char> & v)
{
    v.resize(32);
    for(int i = 0; i <= 15; i++)
        v[i] = (char)(i+32);
    for(int i = 16; i <= 22; i++)
        v[i] = (char)(i+42);
    for(int i = 23; i <= 27; i++)
        v[i] = (char)(i+68);
    for(int i = 28; i <= 31; i++)
        v[i] = (char)(i+95);
}

string encrypt(const string & s, const string & master)
{
    string encrypted;
    StringSource ss1(s, true,
        new DefaultEncryptorWithMAC(
            (byte*)master.data(), master.size(),
            new HexEncoder(
                new StringSink(encrypted)
            )
        )
    );
    return encrypted;
}

string decrypt(const string & s, const string & master)
{
    string decrypted;
    StringSource ss2(s, true,
        new HexDecoder(
            new DefaultDecryptorWithMAC(
            (byte*)master.data(), master.size(),
                new StringSink(decrypted)
            )
        )
    );
    return decrypted;
}

void print(string s, int lenmax)
{
    if(lenmax < (int)s.length())
        for(int i = 0; i < lenmax; i++)
            cout << s[i];
    else
        cout << s;
    cout << '\n';
}*/
#include "encrypt.h"
#include "sodium.h"
#include <vector>

std::string generatePassword(int len, char_options options)
{
    std::vector<char> alphabet;
    if(options.lowercase)
        for(char c = 'a'; c <= 'z'; c++)
            alphabet.push_back(c);
    if(options.uppercase)
        for(char c = 'A'; c <= 'Z'; c++)
            alphabet.push_back(c);
    if(options.numbers)
        for(char c = '0'; c <= '9'; c++)
            alphabet.push_back(c);
    if(options.symbols)
    {
        for(char c = 33; c <= 47; c++)
            alphabet.push_back(c);
        for(char c = 58; c <= 64; c++)
            alphabet.push_back(c);
        for(char c = 91; c <= 96; c++)
            alphabet.push_back(c);
        for(char c = 123; c <= 126; c++)
            alphabet.push_back(c);
    }

    std::string pass;
    for(int i = 0; i < len; i++)
    {
        int index = randombytes_uniform(alphabet.size());
        pass += alphabet[index];
    }
    return pass;
}
