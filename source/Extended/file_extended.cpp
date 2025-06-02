#include "file_extended.h"

void connect(std::fstream& stream, const std::string& file) {
    while (!stream.is_open()) {
        stream.open(file, std::ios::in | std::ios::out | std::ios::binary);
        if (!stream.is_open()) {
            std::cout << "Connect " + file + " failed!" << std::endl;
            std::cout << "Creating " + file + " file..." << std::endl;
            stream.close();
            stream.open(file, std::ios::out);
            stream.close();
            std::cout << "Reconnect..." << std::endl;
        }
    }
    std::cout << "Connect " + file + " successfully!" << std::endl;
}

void close(std::fstream& stream, const std::string& file){
    stream.close();
    std::cout << "Disconnect file " + file + "!" << std::endl;
}