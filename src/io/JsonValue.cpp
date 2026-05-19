#include "../../include/io/JsonValue.hpp"
#include "../../include/exceptions/AppError.hpp"

JsonValue::JsonValue() : _data(nullptr) {}
JsonValue::JsonValue(std::nullptr_t) : _data(nullptr) {}
JsonValue::JsonValue(bool b) : _data(b) {}
JsonValue::JsonValue(int i) : _data(static_cast<double>(i)) {}
JsonValue::JsonValue(double d) : _data(d) {}
JsonValue::JsonValue(const char* s) : _data(std::string(s)) {}
JsonValue::JsonValue(std::string s) : _data(std::move(s)) {}
JsonValue::JsonValue(Array a) : _data(std::move(a)) {}
JsonValue::JsonValue(Object o) : _data(std::move(o)) {}

bool JsonValue::isNull()   const { return std::holds_alternative<std::nullptr_t>(_data); }
bool JsonValue::isBool()   const { return std::holds_alternative<bool>(_data); }
bool JsonValue::isNumber() const { return std::holds_alternative<double>(_data); }
bool JsonValue::isString() const { return std::holds_alternative<std::string>(_data); }
bool JsonValue::isArray()  const { return std::holds_alternative<Array>(_data); }
bool JsonValue::isObject() const { return std::holds_alternative<Object>(_data); }

bool JsonValue::asBool() const {
    if(!isBool()) throw AppError("JSON value is not a bool (is " + typeName() + ")", "JSON_TYPE");
    return std::get<bool>(_data);
}

double JsonValue::asNumber() const {
    if(!isNumber()) throw AppError("JSON value is not a number (is " + typeName() + ")", "JSON_TYPE");
    return std::get<double>(_data);
}

const std::string& JsonValue::asString() const {
    if(!isString()) throw AppError("JSON value is not a string (is " + typeName() + ")", "JSON_TYPE");
    return std::get<std::string>(_data);
}

const JsonValue::Array& JsonValue::asArray() const {
    if(!isArray()) throw AppError("JSON value is not an array (is " + typeName() + ")", "JSON_TYPE");
    return std::get<Array>(_data);
}

JsonValue::Array& JsonValue::asArray() {
    if(!isArray()) throw AppError("JSON value is not an array (is " + typeName() + ")", "JSON_TYPE");
    return std::get<Array>(_data);
}

const JsonValue::Object& JsonValue::asObject() const {
    if(!isObject()) throw AppError("JSON value is not an object (is " + typeName() + ")", "JSON_TYPE");
    return std::get<Object>(_data);
}

JsonValue::Object& JsonValue::asObject() {
    if(!isObject()) throw AppError("JSON value is not an object (is " + typeName() + ")", "JSON_TYPE");
    return std::get<Object>(_data);
}

bool JsonValue::has(const std::string& key) const {
    return isObject() && asObject().count(key) > 0;
}

const JsonValue& JsonValue::at(const std::string& key) const {
    const Object& obj = asObject();
    const auto it = obj.find(key);
    if(it == obj.end()){
        throw AppError("Missing JSON key: '" + key + "'", "JSON_MISSING_KEY");
    }
    return it->second;
}

std::string JsonValue::typeName() const {
    if(isNull())   return "null";
    if(isBool())   return "bool";
    if(isNumber()) return "number";
    if(isString()) return "string";
    if(isArray())  return "array";
    if(isObject()) return "object";
    return "unknown";
}
