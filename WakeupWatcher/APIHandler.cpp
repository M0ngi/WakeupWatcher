#include "Client.h"
#include <string>
#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <atlstr.h>
#include "Response.h"
#include <locale>
#include "utils.cpp"

#pragma comment(lib, "cpprest141_2_10.lib")

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using std::wstring;

Response Client::getAPI(wstring path) {
	http_client client(L"https://discord.com/api/v9/"+ path);
	http_request req(methods::GET);
	req.headers().add(L"Authorization", (L"Bot "+cfg->TOKEN).c_str());
	req.headers().set_content_type(L"application/json");

	http_response response = client.request(req).get();
	response.headers().set_content_type(L"application/json");

	auto body = response.extract_string().get();
	Response r(response.status_code(), json::value::parse(body), this);
	return r;
}

Response Client::postAPI(wstring path, json::value data) {
	http_client client(L"https://discord.com/api/v9/" + path);
	http_request req(methods::POST);
	req.headers().add(L"Authorization", (L"Bot " + cfg->TOKEN).c_str());
	req.headers().set_content_type(L"application/json");
	
	req.set_body(data);

	http_response response = client.request(req).get();
	response.headers().set_content_type(L"application/json");

	auto body = response.extract_string().get();
	Response r(response.status_code(), json::value::parse(body), this);
	return r;
}

Response Client::postImageAPI(wstring path, string msg, string fName) {
	std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > conv;

	http_client client(L"https://discord.com/api/v9/" + path);
	http_request req(methods::POST);
	req.headers().add(L"Authorization", (L"Bot " + cfg->TOKEN).c_str());

	web::json::value obj = web::json::value::object();
	obj[U("content")] = web::json::value::string(conv.from_bytes(msg));

	std::string boundary = "KabezOfFireDegelDegelGoGoniDaijobo";
	std::stringstream bodyContent;

	bodyContent << "--" << boundary << "\r\n"
		<< "Content-Disposition: form-data; name=\"payload_json\"\r\n"
		<< "Content-Type: application/json\r\n\r\n"
		<< utility::conversions::to_utf8string(obj.serialize()) << "\r\n";


	bodyContent << "--" << boundary << "\r\n"
		<< "Content-Disposition: form-data; name=\"files[0]\"; filename=\"file.jpg\"\r\n"
		<< "Content-Type: image/jpg\r\n\r\n"
		<< readFileContent(fName) << "\r\n\r\n";

	bodyContent << "--" << boundary << "--";

	req.set_body(bodyContent.str(), "multipart/form-data; boundary=" + boundary);
	http_response response = client.request(req).get();

	auto body = response.extract_string().get();

	Response r(response.status_code(), json::value::parse(body), this);
	return r;
}