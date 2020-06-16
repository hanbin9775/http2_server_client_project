#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <string>
#include <iostream>

class Subscriber 
{
private:
    std::string first_name;
    std::string last_name;
    std::string phone_number;
    std::string addressp;
public:
    Subscriber();
    ~Subscriber();

    //Constructor
    Subscriber(std::string fname,std::string lname,std::string number,std::string address);

    //setter
    void setFirstName(std::string fname);
    void setLastName(std::string lname);
    void setPhoneNumber(std::string number);
    void setAddressp(std::string address);

    //getter
    std::string getFirstName();
    std::string getLastName();
    std::string getPhoneNumber();
    std::string getAddressp();

};

#endif