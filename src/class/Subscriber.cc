#include "Subscriber.h"

//Constructor
Subscriber::Subscriber(){}
Subscriber::Subscriber(std::string fname,std::string lname,std::string number,std::string address){
    first_name = fname;
    last_name = lname;
    phone_number = number;
    addressp = address;
}

//Destructor
Subscriber::~Subscriber(){}

//setter
void Subscriber::setFirstName(std::string fname){
    first_name = fname;
}

void Subscriber::setLastName(std::string lname){
    last_name = lname;
}

void Subscriber::setPhoneNumber(std::string number){
    phone_number = number;
}

void Subscriber::setAddressp(std::string address){
    addressp = address;
}

//getter
std::string Subscriber::getFirstName(){
    return first_name;
}

std::string Subscriber::getLastName(){
    return last_name;   
}

std::string Subscriber::getPhoneNumber(){
    return phone_number;
}

std::string Subscriber::getAddressp(){
    return addressp;
}