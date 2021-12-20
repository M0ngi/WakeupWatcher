#pragma once

#include "Response.h"
#include <cpprest/json.h>
#include <string>

using std::string;
using std::wstring;
class Message;

class Channel : public Response {
public:
    wstring id;
    Message sendMessage(string msg); // Can be changed later to return a Message object
    Message sendImage(string, string="");

    Channel(int, web::json::value, Client*, wstring);
};