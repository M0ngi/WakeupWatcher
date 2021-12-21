#pragma once
#include <string>
#include <cpprest/json.h>
#include "Response.h"
#include "Channel.h"
#include "Message.h"
#include "Config.h"

using std::wstring;
using std::string;

class Client
{
    private:
        bool ready = false;
    public:
        Config* cfg;

        Response getAPI(wstring);
        Response postAPI(wstring, web::json::value);
        Response postImageAPI(wstring, string, string);

        Channel getChannel(string);
        Channel getChannelByID(string);

        Client(Config*);

        int checkChannels();
};

