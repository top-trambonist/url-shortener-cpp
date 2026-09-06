#include <crow.h>

int main()
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([]()
	{
			return "Hello from C++!";
	});

	app.port(8080).run();
}