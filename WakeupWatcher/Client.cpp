#include "Client.h"
#include <string>
#include <iostream>
#include "Config.h"

using std::string;

Client::Client(Config* cfg)
{
    this->cfg = cfg;

    getAPI(L"channels/" + cfg->IMG_CHANN);
}

void Client::hello() {
    std::cout << "hi";
}