#include "../../include/io/JsonWriter.hpp"
#include "../../include/exceptions/AppError.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

std::string JsonWriter::write(const JsonValue& v, bool pretty){
    std::ostringstream os;
    writeTo(os, v, pretty);
    return os.str();
}

void JsonWriter::writeFile(const std::string& path, const JsonValue& v, bool pretty){
    std::ofstream out(path);
    if(!out){
        throw AppError("could not open file for writing: '" + path + "'", "JSON_WRITE");
    }
    writeTo(out, v, pretty);
    out << '\n';
}

void JsonWriter::writeTo(std::ostream& os, const JsonValue& v, bool pretty){
    writeValue(os, v, pretty, 0);
}

void JsonWriter::writeIndent(std::ostream& os, int indent){
    for(int i = 0; i < indent; ++i) os << "  ";
}

void JsonWriter::writeString(std::ostream& os, const std::string& s){
    os << '"';
    for(const char c : s){
        switch(c){
            case '"':  os << "\\\""; break;
            case '\\': os << "\\\\"; break;
            case '\b': os << "\\b";  break;
            case '\f': os << "\\f";  break;
            case '\n': os << "\\n";  break;
            case '\r': os << "\\r";  break;
            case '\t': os << "\\t";  break;
            default:
                if(static_cast<unsigned char>(c) < 0x20){
                    os << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                       << static_cast<int>(static_cast<unsigned char>(c))
                       << std::dec << std::setfill(' ');
                } else {
                    os << c;
                }
        }
    }
    os << '"';
}

void JsonWriter::writeNumber(std::ostream& os, double d){
    if(std::isnan(d) || std::isinf(d)){
        os << "null";
        return;
    }
    if(d == std::floor(d) && std::abs(d) < 1e16){
        os << static_cast<long long>(d);
    } else {
        std::ostringstream tmp;
        tmp.precision(15);
        tmp << d;
        os << tmp.str();
    }
}

void JsonWriter::writeValue(std::ostream& os, const JsonValue& v, bool pretty, int indent){
    if(v.isNull())   { os << "null"; return; }
    if(v.isBool())   { os << (v.asBool() ? "true" : "false"); return; }
    if(v.isNumber()) { writeNumber(os, v.asNumber()); return; }
    if(v.isString()) { writeString(os, v.asString()); return; }
    if(v.isArray()){
        const auto& arr = v.asArray();
        if(arr.empty()){ os << "[]"; return; }
        os << '[';
        for(std::size_t i = 0; i < arr.size(); ++i){
            if(pretty){ os << '\n'; writeIndent(os, indent + 1); }
            writeValue(os, arr[i], pretty, indent + 1);
            if(i + 1 < arr.size()) os << ',';
        }
        if(pretty){ os << '\n'; writeIndent(os, indent); }
        os << ']';
        return;
    }
    if(v.isObject()){
        const auto& obj = v.asObject();
        if(obj.empty()){ os << "{}"; return; }
        os << '{';
        std::size_t i = 0;
        for(const auto& [key, val] : obj){
            if(pretty){ os << '\n'; writeIndent(os, indent + 1); }
            writeString(os, key);
            os << (pretty ? ": " : ":");
            writeValue(os, val, pretty, indent + 1);
            if(++i < obj.size()) os << ',';
        }
        if(pretty){ os << '\n'; writeIndent(os, indent); }
        os << '}';
        return;
    }
}
