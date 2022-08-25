#include <iostream>
#include <crow.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace crow;

struct MiddleWare
{
	struct context
    {};

	void before_handle(crow::request& req, crow::response& res, context& ctx)
    {}

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {}
};

App<MiddleWare> app;
void Routes();

int main()
{
	Routes();

	CROW_CATCHALL_ROUTE(app);

	//app.port(80).multithreaded().run(); // 테스트
	app.port(443).ssl_file("/etc/letsencrypt/live/kmj36.duckdns.org/cert.pem", "/etc/letsencrypt/live/kmj36.duckdns.org/privkey.pem").multithreaded().run();
}

void Routes()
{
	CROW_ROUTE(app, "/").methods(HTTPMethod::GET)
	([]()
	{
		response res(mustache::load_text("index.html"));
		return res;
	});

	CROW_ROUTE(app, "/post/<uint>").methods(HTTPMethod::GET)
	([](unsigned int pagenum)
	{
		response res(::std::to_string(pagenum));
		return res;
	});

	CROW_ROUTE(app, "/search/<string>").methods(HTTPMethod::GET)
	([](::std::string strSearch)
	{
		response res(mustache::load_text("search.html"));
		return res;
	});
}