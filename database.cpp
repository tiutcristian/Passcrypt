#include "database.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include "encrypt.h"

Database::Entry::Entry(): Entry("", "", "", "")
{

}

Database::Entry::Entry(std::string title, std::string username, std::string description, std::string pass)
{
    this->title = title;
    this->username = username;
    this->description = description;
    this->pass = pass;
}

Database::Database(bool isNew, std::string masterPassword)
{
    this->masterPassword = masterPassword;
    if(!isNew)
        read();
}

void Database::add(const Entry &entry)
{
    entries.push_back(entry);
    sortEntries();
    save();
}

void Database::remove(const Entry &entry)
{
    std::string crtTitle = entry.title;
    std::vector<Entry>::iterator pos;
    for(auto i = entries.begin(); i < entries.end(); i++)
        if(i->title == crtTitle)
            pos = i;
    entries.erase(pos);
    save();
}

void Database::read()
{
    try
    {
        std::stringstream fin (decrypt("database.txt", masterPassword));
        std::string name, id, description, pass;
        entries.clear();
        while(getline(fin, name))
        {
            getline(fin, id);
            getline(fin, description);
            getline(fin, pass);
            entries.push_back(Entry(name, id, description, pass));
        }
    }
    catch(...)
    {
        throw BadPasswordException();
    }
}

void Database::save()
{
    std::stringstream fout;
    for(const auto &elem : entries)
        fout << elem.title << '\n'
             << elem.username << '\n'
             << elem.description << '\n'
             << elem.pass << '\n';
    encrypt(fout.str(), masterPassword, "database.txt");
}

bool Database::availableTitle(std::string title, std::vector<Entry> entries, int indexToSkip)
{
    if(title.empty())
        return false;
    for(int i = 0; i < indexToSkip; i++)
        if(entries[i].title == title)
            return false;
    for(int i = indexToSkip+1; i < (int)entries.size(); i++)
        if(entries[i].title == title)
            return false;
    return true;
}

bool Database::entriesCmp(const Entry &a, const Entry &b)
{
    if(a.title.compare(b.title) < 0)
        return true;
    if(a.title.compare(b.title) == 0 && a.username.compare(b.username) < 0)
        return true;
    return false;
}

void Database::sortEntries()
{
    sort(entries.begin(), entries.end(), entriesCmp);
}
