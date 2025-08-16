#ifndef DATE_H
#define DATE_H
#include "FileTransferElement.h"
namespace MyBase  {
    class Date: public FileTransferElement {
    public:
        Date();
        Date(const unsigned int& year, const unsigned char& month, const unsigned char& day);
        ~Date();

        static Date today();
        Date& operator=(const Date&) const = delete; 
        const unsigned char& getDay() const,& getMonth() const;
        const unsigned int& getYear() const;
    protected:
    private:
        unsigned char __day, __month;
        unsigned int  __year;
        virtual void input(File& file) override;
        virtual void output(File& file) const override;

    };
}
#endif