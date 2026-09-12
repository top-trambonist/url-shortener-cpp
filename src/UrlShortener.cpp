#include <crow.h>
#include "Shortener.h"
#include <iostream>
#include <sqlite3.h>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

bool isValidUrl(const std::string& url) {
	return url.starts_with("http://") ||
		url.starts_with("https://");
}

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	std::filesystem::create_directories("data");
	UrlRepository repository("data/urls.db");

	Shortener shortener(repository);

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([]()
	{
			crow::response response;

			response.set_static_file_info("web/index.html");

			return response;
	});

	CROW_ROUTE(app, "/static/css/<string>")
		([](std::string filename)
			{
				crow::response response;

				response.set_static_file_info(
					"web/css/" + filename
				);

				return response;
			});

	CROW_ROUTE(app, "/static/images/<string>")
		([](std::string filename)
			{
				crow::response response;

				response.set_static_file_info(
					"web/images/" + filename
				);

				return response;
			});

	CROW_ROUTE(app, "/shorten")
		.methods(crow::HTTPMethod::POST)
		([&shortener](const crow::request& req)
			{
				const auto params = req.get_body_params();
				char* url = params.get("url");

				if (url == nullptr || url[0] == '\0') {
					return crow::response(
						400,
						"<p>URL потерян.</p>"
					);
				}

				std::string originalUrl = url;
				if (!isValidUrl(originalUrl)) {
					return crow::response(
						400,
						"<p>Некорректный URL.</p>"
					);
				}
				
				std::string code = shortener.shorten(originalUrl);

				std::cout
					<< "Оригинальная URL: "
					<< originalUrl
					<< '\n'
					<< "Сгенерированная URL: "
					<< code
					<< '\n';

				std::string shortUrl =
					"http://localhost:8080/" + code;

				std::string responseText =
					"<p>Ваша короткая ссылка:</p>"
					"<input id=\"short-url\" value=\"" +
					shortUrl +
					"\" readonly>"
					"<button class=\"copy-button\" onclick=\"copyLink()\">"
					"Копировать"
					"</button>";

				crow::response response(
					200,
					responseText
				);

				response.set_header(
					"Content-Type",
					"text/html; charset=UTF-8"
				);

				return response;
			});

	CROW_ROUTE(app, "/<string>")
		([&shortener](const std::string& code) {
			std::string originalUrl = shortener.getOriginalUrl(code);

			if (originalUrl.empty()) {
				return crow::response(
					404,
					"<p>Короткая ссылка не найдена.</p>"
				);
			}

			crow::response response;
			response.code = 302;
			response.set_header("Location", originalUrl);

			return response;
			});

	app.bindaddr("0.0.0.0").port(8080).run();
}