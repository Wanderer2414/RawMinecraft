#include "Id.h"

Id::Id(const size_t& id) {
    m_id = id;
}
Id::~Id() {

}
string Id::to_string() const {
    return std::to_string(m_id);
}
int Id::operator[](const size_t& index) const {
    return (m_id/int(pow(10, index)))%10;
}

Id::operator bool() const {
    return m_id;
}
Id::operator size_t() const {
    return m_id;
}

bool operator<(const Id& a, const Id& b) {
    return a.m_id < b.m_id;
}

bool operator>(const Id& a, const Id& b) {
    return a.m_id > b.m_id;
}
bool operator!=(const Id& a, const Id& b) {
    return a.m_id!=b.m_id;
}
bool operator==(const Id& a, const Id& b) {
    return a.m_id==b.m_id;
}
void Id::operator+=(const int& offset) {
    m_id = (m_id+offset)%int(pow(10, size));
}
ostream& operator<<(ostream& out, const Id& id) {
    out << id.m_id;
    return out;
}