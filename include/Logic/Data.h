#ifndef DATA_H
#define DATA_H
#include "Global.h"
#include "Id.h"
class Data {
public:
    enum Type {
        string, number, vertex
    };
    virtual size_t size() const = 0;
    virtual void get(char*) const = 0;
    static Data* fetch(char*);
    virtual std::string to_string() const = 0;
    virtual Type type() const = 0;
    virtual void free() = 0;
private:
};

class StringData: public Data {
public:
    StringData(const std::string& value = "");
    ~StringData();
    virtual size_t size() const override;
    virtual void get(char*) const override;
    static StringData* fetch(char*);
    std::string data;
    virtual Type type() const override;
    virtual std::string to_string() const override;
    virtual void free() override;
private:
};

class VertexData: public Data {
public:
    VertexData(const Id& id = Id(0));
    ~VertexData();
    Id id;
    static VertexData* fetch(char*);
    virtual size_t size() const override;
    virtual void get(char*) const override;
    virtual Type type() const override;
    virtual std::string to_string() const override;
    virtual void free() override;
};
#endif