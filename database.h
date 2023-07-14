#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database
{
public:
    // constructor
    Database(bool isNew, std::string masterPassword);

    // Entry class
    class Entry{
    public:
        std::string title, username, description, pass;
        Entry();
        Entry(std::string title, std::string username, std::string description, std::string pass);
    };

    // bad password exception
    class BadPasswordException{};

    // entries vector
    std::vector<Entry> entries;

    // database methods
    void add(const Entry &entry);
    void remove(const Entry &entry);
    void save();

    // functions
    bool availableTitle(std::string title, std::vector<Database::Entry> entries, int indexToSkip);

private:

    // variables
    std::string masterPassword;

    // functions
    static bool entriesCmp(const Entry &a, const Entry &b);

    // database methods
    void sortEntries();
    void read();

};

#endif // DATABASE_H
