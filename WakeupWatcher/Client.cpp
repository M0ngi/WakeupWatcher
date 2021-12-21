#include "Client.h"
#include <string>
#include <iostream>
#include <locale>
#include <cpprest/http_client.h> // For exceptions

using std::string;
using std::wstring;

Client::Client(Config* cfg)
{
    this->cfg = cfg;
}


Channel Client::getChannelByID(string id) {
    std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > conv;

    Response r = getAPI(conv.from_bytes("channels/" + id));
    return Channel(r.response_code, r.resp, this, conv.from_bytes(id));;
}

Channel Client::getChannel(string name) {
    if (name == "log") { 
        Response r = getAPI(L"channels/" + cfg->LOG_CHANN);
        return Channel(r.response_code, r.resp, this, cfg->LOG_CHANN);
    }
    else if (name == "img"){ 
        Response r = getAPI(L"channels/" + cfg->IMG_CHANN);
        return Channel(r.response_code, r.resp, this, cfg->IMG_CHANN);
    }
    else if (name == "txt"){ 
        Response r = getAPI(L"channels/" + cfg->TXT_CHANN);
        return Channel(r.response_code, r.resp, this, cfg->TXT_CHANN);
    }
    else { 
        Response r = getAPI(L"channels/" + cfg->ERR_CHANN); 
        return Channel(r.response_code, r.resp, this, cfg->ERR_CHANN);
    }
}

int Client::checkChannels() {
    try {
        if (getAPI(L"channels/" + cfg->IMG_CHANN).response_code != 200) {
            std::cout << "Error: IMG Channel\n";
            return 0;
        }
        if (getAPI(L"channels/" + cfg->LOG_CHANN).response_code != 200) {
            std::cout << "Error: LOG Channel\n";
            return 0;
        }
        if (getAPI(L"channels/" + cfg->TXT_CHANN).response_code != 200) {
            std::cout << "Error: TXT Channel\n";
            return 0;
        }
        if (getAPI(L"channels/" + cfg->ERR_CHANN).response_code != 200) {
            std::cout << "Error: ERR Channel\n";
            return 0;
        }
    }
    catch (web::http::http_exception) {
        return -1;
    }

    std::cout << "Init success\n";
    return 1;
}
