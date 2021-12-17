#include <string>
#include "Config.h"

#pragma once

using std::wstring;

class Client
{
private:
    Config* cfg;
    bool ready = false;

    void getAPI(wstring);

public:
    Client(Config*);
    void hello();
    // ~client();
};