#ifndef FILE_H
#define FILE_H
#include "Global.h"
#include "FileTransferElement.h"

namespace MyBase {
    class File;
    class FileBuffer: public FileTransferElement {
    public:
        FileBuffer();
        ~FileBuffer();
        char* data;
        size_t size;
    private:
        void input(File& file) override;
        void output(File& file) const override;
    };
    class File {
    public:
        File();
        File(const std::string& file);
        File(const File&) = delete;
        ~File();

        #define oper(T) File& operator>>(T& value);        \
                        File& operator<<(const T& value);  
        
        oper(int);
        oper(char);
        oper(unsigned int);
        oper(unsigned char);
        oper(std::string)
        oper(std::vector<int>);
        oper(std::vector<unsigned char>);

        File& operator<<(const MyBase::FileTransferElement& file);
        File& operator>>(MyBase::FileTransferElement& file);
        
        File& operator=(const File&) = delete;
        bool isNew() const;
        unsigned int size() const;
        void readAt(const unsigned int& position);
        void writeAt(const unsigned int& position);

        const std::string& getFileName() const;
        void connect(const std::string& file);
        void close();
        void read(char* data, const unsigned int& size);
        void write(const char* data, const unsigned int& size);
    private:
        bool __isNew;
        unsigned int __read_position, __write_position;
        std::fstream __file;
        unsigned int __file_size;
        std::string __source;

    };
}
#endif