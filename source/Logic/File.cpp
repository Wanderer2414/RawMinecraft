#include "File.h"

namespace MyBase {

    FileBuffer::FileBuffer() {}
    FileBuffer::~FileBuffer() {}
    void FileBuffer::input(File& file) {
        file.read(data, size);
    };
    void FileBuffer::output(File& file) const {
        file.write(data, size);
    };
    
    #define in(T)                                 \
    File& File::operator<<(const T& num) {          \
        write((char*)&num, sizeof(T));    \
        return *this;                               \
    }                                               \

    #define out(T)                                  \
    File& File::operator>>(T& num) {                \
        read((char*)&num, sizeof(T));   \
        return *this;                               \
    }                                               \
    
    in(int)
    in(unsigned int)
    in(char)
    in(unsigned char)

    out(int)
    out(unsigned int)
    out(char)
    out(unsigned char)

    File& File::operator>>(std::string& value) {
        unsigned int sz = 0;
        read((char*)&sz, sizeof(int));
        value.resize(sz);
        read(value.data(), value.size());
        return *this;
    }

    File& File::operator<<(const std::string& value) {
        unsigned int sz = value.size();
        write((char*)&sz, sizeof(int));
        write((char*)value.data(), value.size());
        return *this;
    }

    File& File::operator>>(std::vector<int>& arr) {
        unsigned int sz = 0;
        read((char*)&sz, sizeof(int));
        arr.resize(sz);
        read((char*)arr.data(), sz*sizeof(int));
        return *this;
    }
    File& File::operator<<(const std::vector<int>& arr) {
        unsigned int size = arr.size();
        write((char*)&size, sizeof(int));
        write((char*)arr.data(), size*sizeof(int));
        return *this;
    }

    File& File::operator>>(std::vector<unsigned char>& arr) {
        unsigned int sz = 0;
        read((char*)&sz, sizeof(int));
        arr.resize(sz);
        read((char*)arr.data(), sz*sizeof(char));
        return *this;
    }
    File& File::operator<<(const std::vector<unsigned char>& arr) {
        unsigned int size = arr.size();
        write((char*)&size, sizeof(int));
        write((char*)arr.data(), size*sizeof(char));
        return *this;
    }

    File& File::operator<<(const FileTransferElement& file) {
        file.output(*this);
        return *this;
    }
    File& File::operator>>(FileTransferElement& file) {
        file.input(*this);
        return *this;
    }
    
    File::File() {}
    File::File(const std::string& file) {
        connect(file);
    }
    File::~File() {
        close();
    }
    bool File::isNew() const {
        return __isNew;
    }
    unsigned int File::size() const {
        return __file_size;
    }
    void File::connect(const std::string& file) {
        close();
        __source = file; 
        __file.open(file, std::ios::binary | std::ios::in | std::ios::out);
        if (!__file.is_open()) {
            __file.open(file, std::ios::out);
            __file.close();
            __file.open(file, std::ios::binary | std::ios::in | std::ios::out);
            writeAt(0);
            readAt(0);
            *this << "MyBase";
            __file_size = 10;
            __isNew = true;
        }
        else {
            __file.seekg(0, std::ios::end);
            __file_size = __file.tellg();
            readAt(0);
            writeAt(0);
            std::string code;
            *this >> code;
            if (code != "MyBase") {
                code = "MyBase";
                __file_size = 10;
                *this << code;
                __isNew = true;
            }
            else __isNew = false;
        }
        __write_position = __read_position = 10;
    }
    void File::close() {
        if (__file.is_open()) {
            __file.close();
        }
    }
    void File::readAt(const unsigned int& position) {
        __read_position = position;
    }
    void File::writeAt(const unsigned int& position) {
        __write_position = position;
    }
    void File::write(const char* data, const unsigned int& sz) {
        __file.seekp(__write_position);
        __file.write(data, sz);
        __write_position+=sz;
        if (__write_position > __file_size) __file_size = __write_position;
    }
    void File::read(char* data, const unsigned int& sz) {
        __file.seekg(__read_position);
        if (__read_position+sz<=__file_size) {
            __file.read(data, sz);
            __read_position += sz;
        }
    }
    const std::string& File::getFileName() const {
        return __source;
    }    
}