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

Shortener::Shortener(UrlRepository& repository)
    : repository(repository) {
}



std::string Shortener::shorten(const std::string& originalUrl) {
    std::lock_guard<std::mutex> lock(mutex);

    auto existingCode = repository.findCodeByUrl(originalUrl);

    if (existingCode) return *existingCode;

    while (true) {
        std::string code = generateCode();
        bool saved = repository.save(
            code,
            originalUrl
        );

        if (saved) return code;

        existingCode = repository.findCodeByUrl(originalUrl);
        if (existingCode) return *existingCode;
    }
}

std::string Shortener::getOriginalUrl(const std::string& code) {
    std::lock_guard<std::mutex> lock(mutex);

    auto url = repository.findUrlByCode(code);
    if (!url) return "";
    return *url;
}