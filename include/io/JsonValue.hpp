#pragma once
#include <cstddef>
#include <map>
#include <string>
#include <variant>
#include <vector>

class JsonValue {
public:
    using Object = std::map<std::string, JsonValue>;
    using Array  = std::vector<JsonValue>;

    JsonValue();                            // null
    JsonValue(std::nullptr_t);
    JsonValue(bool);
    JsonValue(int);
    JsonValue(double);
    JsonValue(const char*);
    JsonValue(std::string);
    JsonValue(Array);
    JsonValue(Object);

    bool isNull()   const;
    bool isBool()   const;
    bool isNumber() const;
    bool isString() const;
    bool isArray()  const;
    bool isObject() const;

    bool                asBool()   const;
    double              asNumber() const;
    const std::string&  asString() const;
    const Array&        asArray()  const;
    const Object&       asObject() const;
    Object&             asObject();
    Array&              asArray();

    bool has(const std::string& key) const;
    const JsonValue& at(const std::string& key) const;

    std::string typeName() const;

private:
    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> _data;
};
