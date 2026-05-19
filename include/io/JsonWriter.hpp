#pragma once
#include <ostream>
#include <string>
#include "./JsonValue.hpp"

class JsonWriter {
public:
    static std::string write(const JsonValue&, bool pretty = true);
    static void writeFile(const std::string& path, const JsonValue&, bool pretty = true);
    static void writeTo(std::ostream& os, const JsonValue&, bool pretty = true);

private:
    static void writeValue(std::ostream& os, const JsonValue&, bool pretty, int indent);
    static void writeString(std::ostream& os, const std::string&);
    static void writeNumber(std::ostream& os, double);
    static void writeIndent(std::ostream& os, int indent);
};
