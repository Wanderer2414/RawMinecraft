#ifndef ID_H
#define ID_H
#include "Global.h"
class IdStorage;
class Id {
public:
    static const int size = 10;
    Id(const size_t& id = 0);
    string to_string() const;
    ~Id();
    operator bool() const;
    operator size_t() const;
    int operator[](const size_t& index) const;
    void operator+=(const int& offset);
    friend bool operator<(const Id& a, const Id& b);
    friend bool operator>(const Id& a, const Id& b);
    friend bool operator!=(const Id& a, const Id& b);
    friend bool operator==(const Id& a, const Id& b);
    friend ostream& operator<<(ostream& out, const Id& id);
    friend IdStorage;
private:
    size_t m_id;
};
bool operator<(const Id& a, const Id& b);
bool operator>(const Id& a, const Id& b);
bool operator!=(const Id& a, const Id& b);
bool operator==(const Id& a, const Id& b);
ostream& operator<<(ostream& out, const Id& id);
#endif