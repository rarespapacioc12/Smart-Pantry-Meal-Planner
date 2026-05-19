#include "../../include/io/JsonReader.hpp"
#include "../../include/exceptions/JsonParseError.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>

JsonReader::JsonReader(const std::string& text)
: _text(text), _pos(0), _line(1), _col(1) {}

JsonValue JsonReader::parse(const std::string& text){
    JsonReader r(text);
    r.skipWhitespace();
    JsonValue v = r.parseValue();
    r.skipWhitespace();
    if(!r.eof()){
        r.fail("trailing characters after top-level value");
    }
    return v;
}

JsonValue JsonReader::parseFile(const std::string& path){
    std::ifstream in(path);
    if(!in){
        throw JsonParseError("could not open file '" + path + "'", 0, 0);
    }
    std::ostringstream buf;
    buf << in.rdbuf();
    return parse(buf.str());
}

void JsonReader::skipWhitespace(){
    while(!eof()){
        const char c = _text[_pos];
        if(c == ' ' || c == '\t' || c == '\r' || c == '\n'){
            consume();
        } else {
            break;
        }
    }
}

char JsonReader::peek() const {
    if(eof()) fail("unexpected end of input");
    return _text[_pos];
}

char JsonReader::consume(){
    if(eof()) fail("unexpected end of input");
    const char c = _text[_pos++];
    if(c == '\n'){ ++_line; _col = 1; }
    else         { ++_col; }
    return c;
}

bool JsonReader::eof() const { return _pos >= _text.size(); }

void JsonReader::fail(const std::string& reason) const {
    throw JsonParseError(reason, _line, _col);
}

void JsonReader::expect(char c, const std::string& context){
    if(eof() || peek() != c){
        fail(std::string("expected '") + c + "' " + context);
    }
    consume();
}

JsonValue JsonReader::parseValue(){
    skipWhitespace();
    if(eof()) fail("expected JSON value");
    const char c = peek();
    if(c == '{') return parseObject();
    if(c == '[') return parseArray();
    if(c == '"') return parseString();
    if(c == '-' || (c >= '0' && c <= '9')) return parseNumber();
    if(c == 't' || c == 'f' || c == 'n') return parseLiteral();
    fail(std::string("unexpected character '") + c + "'");
}

JsonValue JsonReader::parseObject(){
    expect('{', "to start object");
    JsonValue::Object obj;
    skipWhitespace();
    if(!eof() && peek() == '}'){ consume(); return JsonValue(std::move(obj)); }
    while(true){
        skipWhitespace();
        if(eof() || peek() != '"') fail("expected string key in object");
        const std::string key = parseString().asString();
        skipWhitespace();
        expect(':', "after object key");
        skipWhitespace();
        obj.emplace(key, parseValue());
        skipWhitespace();
        if(eof()) fail("unterminated object");
        const char next = consume();
        if(next == ','){ continue; }
        if(next == '}'){ break; }
        fail(std::string("expected ',' or '}' in object, got '") + next + "'");
    }
    return JsonValue(std::move(obj));
}

JsonValue JsonReader::parseArray(){
    expect('[', "to start array");
    JsonValue::Array arr;
    skipWhitespace();
    if(!eof() && peek() == ']'){ consume(); return JsonValue(std::move(arr)); }
    while(true){
        skipWhitespace();
        arr.push_back(parseValue());
        skipWhitespace();
        if(eof()) fail("unterminated array");
        const char next = consume();
        if(next == ','){ continue; }
        if(next == ']'){ break; }
        fail(std::string("expected ',' or ']' in array, got '") + next + "'");
    }
    return JsonValue(std::move(arr));
}

JsonValue JsonReader::parseString(){
    expect('"', "to start string");
    std::string out;
    while(true){
        if(eof()) fail("unterminated string");
        const char c = consume();
        if(c == '"') break;
        if(c == '\\'){
            if(eof()) fail("dangling escape in string");
            const char esc = consume();
            switch(esc){
                case '"':  out.push_back('"');  break;
                case '\\': out.push_back('\\'); break;
                case '/':  out.push_back('/');  break;
                case 'b':  out.push_back('\b'); break;
                case 'f':  out.push_back('\f'); break;
                case 'n':  out.push_back('\n'); break;
                case 'r':  out.push_back('\r'); break;
                case 't':  out.push_back('\t'); break;
                default:   fail(std::string("invalid escape '\\") + esc + "'");
            }
        } else {
            out.push_back(c);
        }
    }
    return JsonValue(std::move(out));
}

JsonValue JsonReader::parseNumber(){
    const std::size_t start = _pos;
    if(peek() == '-') consume();
    while(!eof()){
        const char c = peek();
        const bool isPart = (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-';
        if(!isPart) break;
        consume();
    }
    const std::string token = _text.substr(start, _pos - start);
    char* end = nullptr;
    const double v = std::strtod(token.c_str(), &end);
    if(end != token.c_str() + token.size()){
        fail("invalid number '" + token + "'");
    }
    return JsonValue(v);
}

JsonValue JsonReader::parseLiteral(){
    const auto matches = [&](const std::string& word){
        if(_pos + word.size() > _text.size()) return false;
        for(std::size_t i = 0; i < word.size(); ++i){
            if(_text[_pos + i] != word[i]) return false;
        }
        for(std::size_t i = 0; i < word.size(); ++i) consume();
        return true;
    };
    if(matches("true"))  return JsonValue(true);
    if(matches("false")) return JsonValue(false);
    if(matches("null"))  return JsonValue(nullptr);
    fail("expected literal (true/false/null)");
}
