#include "Channel.h"
#include <string>
#include <locale>
#include "Client.h"
#include "Message.h"
#include "Response.h"
#include <fstream>

Channel::Channel(int c, web::json::value resp, Client* source, wstring c_id) : Response(c, resp, source) {
    id = c_id;
}

Message Channel::sendMessage(string msg) {
    if (response_code != 200) return Message(response_code, resp, source, this);

    std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > conv;

    web::json::value obj = web::json::value::object();
    obj[U("content")] = web::json::value::string(conv.from_bytes(msg));


    Response r = source->postAPI(L"channels/" + this->id + L"/messages", obj);
    return Message(r.response_code, r.resp, r.source, this);
}

Message Channel::sendImage(string fName, string msg) {
    if (response_code != 200) return Message(response_code, resp, source, this);

    Response r = source->postImageAPI(L"channels/" + this->id + L"/messages", msg, fName);
    return Message(response_code, resp, source, this);
}