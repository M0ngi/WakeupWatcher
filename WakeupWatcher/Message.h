#pragma once
#include "Response.h"
#include "Channel.h"
#include <string>

using std::wstring;

class Message :
    public Response
{
public:
    Channel* channel;
    Message(int code, web::json::value, Client*, Channel* c);
};

