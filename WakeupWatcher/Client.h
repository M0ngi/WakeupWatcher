#include <string>

#pragma once

using std::string;

class Client
{
private:
    string token;

public:
    Client(string token);
    void hello();
    // ~client();
};