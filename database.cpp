#include "database.h"
#include <fstream>
#include <algorithm>

Database::Entry::Entry(): Entry("", "", "", "")
{

}

Database::Entry::Entry(std::string name, std::string id, std::string description, std::string pass)
{
    this->name = name;
    this->id = id;
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

void Database::read()
{
    if(masterPassword == "")
    {
        std::ifstream fin("database.txt");
        std::string name, id, description, pass;
        entries.clear();
        while(getline(fin, name))
        {
            getline(fin, id);
            getline(fin, description);
            getline(fin, pass);
            entries.push_back(Entry(name, id, description, pass));
        }
        fin.close();
    }
    else
        throw BadPasswordException();
}

void Database::save()
{
    std::ofstream fout("database.txt");
    for(const auto &elem : entries)
        fout << elem.name << '\n'
             << elem.id << '\n'
             << elem.description << '\n'
             << elem.pass << '\n';
    fout.close();
}

bool Database::entriesCmp(const Entry &a, const Entry &b)
{
    if(a.name.compare(b.name) < 0)
        return true;
    if(a.name.compare(b.name) == 0 && a.id.compare(b.id) < 0)
        return true;
    return false;
}

void Database::sortEntries()
{
    sort(entries.begin(), entries.end(), entriesCmp);
}
