#include "Response.h"
#include <cpprest/json.h>

Response::Response(int code, web::json::value d, Client* c) {
    response_code = code;
    resp = d;
    source = c;
}