#include "UrlRepository.h"
#include <stdexcept>

UrlRepository::UrlRepository(const std::string& databasePath) {
	int result = sqlite3_open(
		databasePath.c_str(),
		&database
	);

	if (result != SQLITE_OK) {
		std::string errorMessage = sqlite3_errmsg(database);
		sqlite3_close(database);
		database = nullptr;

		throw std::runtime_error(
			"Failed to open database: " +
			errorMessage
		);
	}

	createTable();
}

UrlRepository::~UrlRepository() {
	if (database != nullptr) sqlite3_close(database);
}

void UrlRepository::createTable() {
	const char* sql = 
		"CREATE TABLE IF NOT EXISTS urls ("
		"code TEXT PRIMARY KEY,"
		"original_url TEXT NOT NULL UNIQUE"
		");";

	char* errorMessege = nullptr;

	int result = sqlite3_exec(
		database,
		sql,
		nullptr,
		nullptr,
		&errorMessege
	);

	if (result != SQLITE_OK) {
		std::string messege =
			errorMessege != nullptr ? errorMessege : "Unknown SQLite error";

		sqlite3_free(errorMessege);

		throw std::runtime_error(
			"Failed to change table: " + messege
		);
	}
}

std::optional<std::string> UrlRepository::findUrlByCode(
	const std::string& code) {
	const char* sql = 
		"SELECT original_url "
		"FROM urls "
		"WHERE code = ?;";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(
		database,
		sql,
		-1,
		&statement,
		nullptr
	);

	if (result != SQLITE_OK) {
		throw std::runtime_error(sqlite3_errmsg(database));
	}

	sqlite3_bind_text(
		statement,
		1,
		code.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	result = sqlite3_step(statement);

	if (result == SQLITE_ROW) {
		const unsigned char* text =
			sqlite3_column_text(
				statement, 0);
		std::string url = reinterpret_cast<const char*>(text);

		sqlite3_finalize(statement);
		return url;
	}

	if (result != SQLITE_DONE) {
		std::string messege = sqlite3_errmsg(database);
		sqlite3_finalize(statement);
		throw std::runtime_error(messege);
	}

	sqlite3_finalize(statement);
	return std::nullopt;
}

std::optional<std::string>
UrlRepository::findCodeByUrl(
	const std::string& url
)
{
	const char* sql =
		"SELECT code "
		"FROM urls "
		"WHERE original_url = ?;";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(
		database,
		sql,
		-1,
		&statement,
		nullptr
	);

	if (result != SQLITE_OK)
	{
		throw std::runtime_error(
			sqlite3_errmsg(database)
		);
	}

	sqlite3_bind_text(
		statement,
		1,
		url.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	result = sqlite3_step(statement);

	if (result == SQLITE_ROW)
	{
		const unsigned char* text =
			sqlite3_column_text(
				statement,
				0
			);

		std::string code =
			reinterpret_cast<const char*>(text);

		sqlite3_finalize(statement);

		return code;
	}

	if (result != SQLITE_DONE)
	{
		std::string message =
			sqlite3_errmsg(database);

		sqlite3_finalize(statement);

		throw std::runtime_error(message);
	}

	sqlite3_finalize(statement);

	return std::nullopt;
}

bool UrlRepository::save(
	const std::string& code,
	const std::string& url
) {
	const char* sql =
		"INSERT OR IGNORE INTO urls "
		"(code, original_url) "
		"VALUES (?, ?);";

	sqlite3_stmt* statement = nullptr;

	int result = sqlite3_prepare_v2(
		database,
		sql,
		-1,
		&statement,
		nullptr
	);

	if (result != SQLITE_OK)
	{
		throw std::runtime_error(
			sqlite3_errmsg(database)
		);
	}

	sqlite3_bind_text(
		statement,
		1,
		code.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	sqlite3_bind_text(
		statement,
		2,
		url.c_str(),
		-1,
		SQLITE_TRANSIENT
	);

	result = sqlite3_step(statement);

	if (result != SQLITE_DONE)
	{
		std::string message =
			sqlite3_errmsg(database);

		sqlite3_finalize(statement);

		throw std::runtime_error(message);
	}

	bool inserted =
		sqlite3_changes(database) > 0;

	sqlite3_finalize(statement);

	return inserted;
}