#include <crow.h>

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

	app.port(8080).run();
}