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
        std::string title, username, description, pass;
        Entry();
        Entry(std::string title, std::string username, std::string description, std::string pass);
    };
    class BadPasswordException{};

    std::vector<Entry> entries;

    void add(const Entry &entry);
    void remove(const Entry &entry);
    void save();
    bool availableTitle(std::string title, std::vector<Database::Entry> entries);

private:
    std::string masterPassword;

    static bool entriesCmp(const Entry &a, const Entry &b);
    void sortEntries();
    void read();

};

#endif // DATABASE_H
