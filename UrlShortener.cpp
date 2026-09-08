#include <crow.h>
#include <iostream>

int main()
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([]()
	{
			crow::response response;

			response.set_static_file_info("index.html");

			return response;
	});

	CROW_ROUTE(app, "/shorten")
		.methods(crow::HTTPMethod::POST)
		([](const crow::request& req)
			{
				const auto params = req.get_body_params();
				char* url = params.get("url");

				if (url == nullptr || url[0] == '\0') {
					return crow::response(
						400,
						"<p>URL is missing.</p>"
					);
				}

				std::cout
					<< "Received URL: "
					<< url
					<< '\n';

				return crow::response(
					200,
					"<p>URL received by C++ server!</p>"
				);
			});

	app.port(8080).run();
}