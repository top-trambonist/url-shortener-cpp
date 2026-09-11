#include "Shortener.h"
#include <random>

std::string Shortener::generateCode() {
    static const std::string characters =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());

    std::uniform_int_distribution<std::size_t> distribution(
        0,
        characters.size() - 1
    );

    std::string code;
    code.reserve(CODE_LENGTH);

    for (int i = 0; i < CODE_LENGTH; i++) {
        code += characters[distribution(generator)];
    }

    return code;
}

std::string Shortener::shorten(const std::string& originalUrl) {
    auto existingUrl = urlToCode.find(originalUrl);

    if (existingUrl != urlToCode.end()) return existingUrl->second;

    std::string code;
    do {
        code = generateCode();
    } while (codeToUrl.find(code) != codeToUrl.end());

    codeToUrl[code] = originalUrl;
    urlToCode[originalUrl] = code;

    return code;
}

std::string Shortener::getOriginalUrl(const std::string& code) {
    auto it = codeToUrl.find(code);

    if (it == codeToUrl.end()) return "";

    return it->second;
}