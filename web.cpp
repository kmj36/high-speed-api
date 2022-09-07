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
    {
		res.add_header("Strict-Transport-Security", "max-age=31536000; includeSubDomains");
		res.add_header("X-XSS-Protection", "1");
		res.add_header("X-Frame-Options", "deny");
		res.add_header("X-Content-Type-Options", "nosniff");
	}
};

App<MiddleWare> app;
void Routes();

int main()
{
	Routes();

	//app.port(80).multithreaded().run(); // 테스트
	app.port(443).ssl_file("/etc/letsencrypt/live/kmj36.duckdns.org/cert.pem", "/etc/letsencrypt/live/kmj36.duckdns.org/privkey.pem").multithreaded().run();
}

void Routes()
{
	CROW_ROUTE(app, "/").methods(HTTPMethod::GET)
	([]()
	{
		response res(mustache::load_text("index.html"));
		res.add_header("Content-Type", "text/html");
		return res;
	});

	CROW_ROUTE(app, "/post/<uint>").methods(HTTPMethod::GET)
	([](unsigned int pagenum)
	{
		response res(::std::to_string(pagenum));
		res.add_header("Content-Type", "text/html");
		return res;
	});

	CROW_ROUTE(app, "/search/<string>").methods(HTTPMethod::GET)
	([](::std::string strSearch)
	{
		response res(mustache::load_text("search.html"));
		res.add_header("Content-Type", "text/html");
		return res;
	});
}