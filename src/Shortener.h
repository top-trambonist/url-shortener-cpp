#pragma once
#include "UrlRepository.h"

#include <mutex>
#include <string>


class Shortener {
public:
	explicit Shortener(UrlRepository& repository);

	std::string shorten(const std::string& originalUrl);
	std::string getOriginalUrl(const std::string& code);
private:
	std::string generateCode();
	static constexpr int CODE_LENGTH = 6;
	
	UrlRepository& repository;
	std::mutex mutex;

};