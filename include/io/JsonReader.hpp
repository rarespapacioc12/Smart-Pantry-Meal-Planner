#pragma once
#include <string>
#include "./JsonValue.hpp"

class JsonReader {
public:
    static JsonValue parse(const std::string& text);
    static JsonValue parseFile(const std::string& path);

private:
    explicit JsonReader(const std::string& text);

    JsonValue parseValue();
    JsonValue parseObject();
    JsonValue parseArray();
    JsonValue parseString();
    JsonValue parseNumber();
    JsonValue parseLiteral();

    void skipWhitespace();
    char peek() const;
    char consume();
    bool eof() const;
    [[noreturn]] void fail(const std::string& reason) const;
    void expect(char c, const std::string& context);

    const std::string& _text;
    std::size_t _pos;
    std::size_t _line;
    std::size_t _col;
};
