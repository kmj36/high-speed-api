#define CROW_ENABLE_COMPRESSION
#define CROW_ENABLE_SSL

#include <iostream>
#include <crow.h>
#include <crow/middlewares/cors.h>
#include <crow/compression.h>
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
		res.add_header("Content-Security-Policy", "default-src 'self'");
	}
};

App<MiddleWare, crow::CORSHandler> app;
void Routes();

int main(int argc, char *argv[])
{
	int LogLevel = 0;
	if(argc != 2)
	{
		std::cout << "Usage: sudo ./" << argv[0] << " [LogLevel: 0~4(DEBUG, INFO, WARNING, ERROR, CRITICAL)]";
		return -1;
	}

	try
	{
		LogLevel = std::stoi(argv[1]);
	}
	catch(std::invalid_argument&)
	{
		std::cout << "Usage: sudo ./" << argv[0] << " [LogLevel: 0~4(DEBUG, INFO, WARNING, ERROR, CRITICAL)]";
		return -1;
	}

	Routes();

	auto& cors = app.get_middleware<crow::CORSHandler>();

	cors.global()
	.headers("Content-Type")
	.methods(HTTPMethod::GET, HTTPMethod::POST)
	.origin("https://kmj36.duckdns.org")
	.allow_credentials();

	app.port(44732)
	.use_compression(crow::compression::algorithm::GZIP)
	.ssl_file("/etc/letsencrypt/live/kmj36.duckdns.org/cert.pem", "/etc/letsencrypt/live/kmj36.duckdns.org/privkey.pem")
	.loglevel((crow::LogLevel)LogLevel)
	.multithreaded()
	.run_async();
}

void Routes()
{
	CROW_ROUTE(app, "/load").methods(HTTPMethod::GET)
	([](const crow::request& req)
	{
		response res;
		res.add_header("Content-Type", "text/html");

		res = req.body;

		return res;
	});

	CROW_ROUTE(app, "/save").methods(HTTPMethod::POST)
	([](const crow::request& req)
	{
		response res;
		res.add_header("Content-Type", "text/html");

		res = req.body;

		return res;
	});

	CROW_ROUTE(app, "/modify").methods(HTTPMethod::POST)
	([](const crow::request& req)
	{
		response res;
		res.add_header("Content-Type", "text/html");

		res = req.body;

		return res;
	});

	CROW_ROUTE(app, "/delete").methods(HTTPMethod::POST)
	([](const crow::request& req)
	{
		response res;
		res.add_header("Content-Type", "text/html");

		res = req.body;
		
		return res;
	});
}