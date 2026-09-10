#pragma once
#include <string>
#include <unordered_map>

class Shortener {
public:
	std::string shorten(const std::string& originalUrl);
	std::string getOriginalUrl(const std::string& code);
private:
	std::string generateCode();
	static constexpr int CODE_LENGTH = 6;
	std::unordered_map<std::string, std::string> urls;
};