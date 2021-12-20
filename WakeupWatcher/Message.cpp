#include "Message.h"

Message::Message(int code, web::json::value resp, Client* src, Channel* c) 
	: Response(code, resp, src)
{
	channel = c;
}
