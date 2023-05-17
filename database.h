#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database
{
public:
    Database(bool isNew, std::string masterPassword);

    class Entry{
    public:
        std::string name, id, description, pass;
        Entry();
        Entry(std::string name, std::string id, std::string description, std::string pass);
    };
    class BadPasswordException{};

    std::vector<Entry> entries;

    void add(const Entry &entry);
    void save();

private:
    std::string masterPassword;

    static bool entriesCmp(const Entry &a, const Entry &b);
    void sortEntries();
    void read();

};

#endif // DATABASE_H
