#include "File.h"

namespace MyBase {

    FileBuffer::FileBuffer() {}
    FileBuffer::~FileBuffer() {}
    void FileBuffer::input(File& file) {
        file.read_from_buffer(data, size);
    };
    void FileBuffer::output(File& file) const {
        file.write_on_buffer(data, size);
    };
    
    #define in(T)                                 \
    File& File::operator<<(const T& num) {          \
        write_on_buffer((char*)&num, sizeof(T));    \
        return *this;                               \
    }                                               \

    #define out(T)                                  \
    File& File::operator>>(T& num) {                \
        read_from_buffer((char*)&num, sizeof(T));   \
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
        read_from_buffer((char*)&sz, sizeof(int));
        value.resize(sz);
        read_from_buffer(value.data(), value.size());
        return *this;
    }

    File& File::operator<<(const std::string& value) {
        unsigned int sz = 0;
        write_on_buffer((char*)&sz, sizeof(int));
        write_on_buffer(value.data(), value.size());
        return *this;
    }

    File& File::operator>>(std::vector<int>& arr) {
        unsigned int sz = 0;
        read_from_buffer((char*)&sz, sizeof(int));
        arr.resize(sz);
        read_from_buffer((char*)arr.data(), sz*sizeof(int));
        return *this;
    }
    File& File::operator<<(const std::vector<int>& arr) {
        unsigned int size = arr.size();
        write_on_buffer((char*)&size, sizeof(int));
        write_on_buffer((char*)arr.data(), size*sizeof(int));
        return *this;
    }

    File& File::operator>>(std::vector<unsigned char>& arr) {
        unsigned int sz = 0;
        read_from_buffer((char*)&sz, sizeof(int));
        arr.resize(sz);
        read_from_buffer((char*)arr.data(), sz*sizeof(char));
        return *this;
    }
    File& File::operator<<(const std::vector<unsigned char>& arr) {
        unsigned int size = arr.size();
        write_on_buffer((char*)&size, sizeof(int));
        write_on_buffer((char*)arr.data(), size*sizeof(char));
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
    unsigned int File::size() const {
        return __file_size;
    }
    void File::connect(const std::string& file) {
        close();
        __offset_read = __offset_write = sizeof(int);
        __write_position = __read_position = __file_size = 0;
        memset(__buffer_write, 0, buffer_size);
        memset(__buffer_read, 0, buffer_size);
        __source = file; 
        __file.open(file, std::ios::binary | std::ios::in | std::ios::out);
        if (!__file.is_open()) {
            __file.open(file, std::ios::out);
            __file.close();
            __file.open(file, std::ios::binary | std::ios::in | std::ios::out);
            writeAt(0);
        }
        else {
            __file.seekg(0, std::ios::end);
            __file_size = __file.tellg();
            readAt(0);
        }
    }
    void File::close() {
        if (__file.is_open()) {
            if (__offset_write>sizeof(int)) __write_on_file();
            __file.close();
        }
    }
    void File::readAt(const unsigned int& position) {
        __read_position = position*buffer_size;
        __read_from_file();
    }
    void File::writeAt(const unsigned int& position) {
        __write_on_file();
        __write_position = position*buffer_size;
        if (__write_position>=__file_size) {
            __file_size = __write_position + buffer_size;
            memset(__buffer_write, 0, sizeof(int));
        }
        else {
            __file.seekg(__write_position);
            __file.read(__buffer_write, buffer_size);
        }
        __file.seekp(__write_position);
        __offset_write = sizeof(int);
    }
    void File::__write_on_file() {
        __file.seekp(__write_position);
        __file.write(__buffer_write, buffer_size);
        __offset_write = sizeof(int);
    }
    void File::write_on_buffer(const char* data, const unsigned int& sz) {
        int memory_size = std::min(sz, buffer_size - __offset_write);
        memcpy(__buffer_write+__offset_write, data, memory_size);
        __offset_write += memory_size;
        if (memory_size!=sz) {
            __sync_read_and_write();
            __write_on_file();
            write_on_buffer(data+memory_size, sz-memory_size);
        }
        else __sync_read_and_write();
    }
    void File::__read_from_file() {
        if (__read_position == __file_size) {
            memset(__buffer_read, 0, buffer_size);
        }
        else {
            __file.seekg(__read_position);
            __file.read(__buffer_read, buffer_size);
        }
        __offset_read = sizeof(int);
        __sync_read_and_write();
        memcpy((char*)&__read_position, __buffer_read, sizeof(int));
        __read_position*=buffer_size;
    }
    void File::read_from_buffer(char* data, const unsigned int& sz) {
        int memory_size = std::min(buffer_size-__offset_read, sz);
        memcpy(data, __buffer_read + __offset_read, memory_size);
        __offset_read += memory_size;
        if (memory_size != sz) {
            __read_from_file();
            read_from_buffer(data+memory_size, sz-memory_size);
        }
    }
    
    void File::__sync_read_and_write() {
        if (__write_position==__read_position && __offset_write>sizeof(int)) {
            memcpy(__buffer_read, __buffer_write, __offset_write);
        }
    }
    const std::string& File::getFileName() const {
        return __source;
    }    
}