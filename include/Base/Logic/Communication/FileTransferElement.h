#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H
namespace MyBase {
    class File;
    class FileTransferElement {
    protected:
        friend class File;
        virtual void input(File& file) = 0;
        virtual void output(File& file) const = 0;
    };
}
#endif