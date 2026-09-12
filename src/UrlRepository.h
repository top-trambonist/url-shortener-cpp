#pragma once
#include <sqlite3.h>

#include <optional>
#include <string>

class UrlRepository {
public:
	explicit UrlRepository(const std::string& databasePath);
	~UrlRepository();

	std::optional<std::string> findCodeByUrl(const std::string& url);

	std::optional<std::string> findUrlByCode(const std::string& code);

	bool save(
		const std::string& code,
		const std::string& url
	);
private:
	void createTable();
	sqlite3* database = nullptr;
};