#include "Client.h"
#include <string>
#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <atlstr.h>

#pragma comment(lib, "cpprest141_2_10.lib")

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using std::wstring;

void Client::getAPI(wstring path) {
	http_client client(L"https://discord.com/api/v9/"+ path);
	http_request req(methods::GET);
	req.headers().add(L"Authorization", (L"Bot "+cfg->TOKEN).c_str());

	http_response response = client.request(req).get();
	auto body = response.extract_string();

	if (response.status_code() == status_codes::OK)
	{
		std::wcout << L"Output: " << body.get().c_str() << std::endl;
	}
	else if (response.status_code() == status_codes::NotFound) {
		std::wcout << L"404: " << body.get().c_str() << std::endl;
	}
	else {
		std::wcout << response.status_code() << L": " << body.get().c_str() << std::endl;
	}

}