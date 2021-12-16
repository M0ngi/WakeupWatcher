#include "Client.h"
#include <string>
#include <iostream>

using std::string;

Client::Client(string token)
{
    this->token = token;

}

void Client::hello() {
    std::cout << "hi";
}