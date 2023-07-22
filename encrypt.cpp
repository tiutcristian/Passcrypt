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
#include <cstring>
#include <cstdlib>
#include <sstream>

#define KEY_LEN crypto_secretstream_xchacha20poly1305_KEYBYTES

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

void getKeyFromPass(std::string pass, unsigned char key[KEY_LEN])
{
    unsigned char salt[crypto_pwhash_SALTBYTES];

    std::memset(salt, 0, crypto_pwhash_SALTBYTES);
    if (crypto_pwhash
        (key, KEY_LEN, pass.c_str(), pass.size(), salt,
         crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
         crypto_pwhash_ALG_DEFAULT) != 0) {
        std::exit(1);
    }
}

void encrypt(const string &s, const string &master, const string &fileName)
{
    unsigned char *buf_in = (unsigned char*)s.c_str();
    unsigned char  buf_out[s.size() + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    unsigned long long out_len;

    FILE *fp_t = fopen(fileName.c_str(), "wb");
    unsigned char key[KEY_LEN];
    getKeyFromPass(master, key);
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
    fwrite(header, 1, sizeof header, fp_t);

    unsigned char tag = crypto_secretstream_xchacha20poly1305_TAG_FINAL;
    crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, s.size(), NULL, 0, tag);
    fwrite(buf_out, 1, (size_t) out_len, fp_t);

    fclose(fp_t);
}

string decrypt(const string &fileName, const string &master)
{
    const int chunkSize = 1024;
    unsigned char  buf_in[chunkSize + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  buf_out[chunkSize];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    unsigned char  tag;
    std::stringstream output;
    FILE *fp_s = fopen(fileName.c_str(), "rb");
    fread(header, 1, sizeof header, fp_s);
    unsigned char key[KEY_LEN];
    getKeyFromPass(master, key);
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key) != 0) {
        fclose(fp_s);
        throw decryptionError(); /* incomplete header */
    }

    do {
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag,
                                                       buf_in, rlen, NULL, 0) != 0) {
            fclose(fp_s);
            throw decryptionError(); /* corrupted chunk */
        }
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL && ! eof) {
            fclose(fp_s);
            throw decryptionError(); /* premature end (end of file reached before the end of the stream) */
        }
        output.write((char*)buf_out, out_len);
    } while (! eof);

    fclose(fp_s);
    return output.str();
}
