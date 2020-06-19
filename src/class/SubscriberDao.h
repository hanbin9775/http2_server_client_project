#ifndef SUBSCRIBERDAO_H
#define SUBSCRIBERDAO_H

#include <string>
#include <iostream>
#include "Subscriber.h"

class SubscriberDao 
{
public:
    SubscriberDao();
    ~SubscriberDao();

    Subscriber bodyParser(std::string req_body);
};

#endif