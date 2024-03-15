#ifndef LUA_TYPE_H
#define LUA_TYPE_H

#include <string>
#include <variant>

namespace cclua {

enum class LuaType {
    kNil,
    kBoolean,
    kNumber,
    kString,
    kFunction,
    kTable,
    kLightUserData,
};

class LuaNil final {
public:
	const LuaType type = LuaType::kNil;
    const std::nullptr_t value = nullptr;

    static LuaNil make() {
		return LuaNil();
	}
private:
    LuaNil() = default;
};

class LuaBoolean final {
public:
    const LuaType type = LuaType::kBoolean;
    const bool value;
    static LuaBoolean make(const bool value) {
		return LuaBoolean(value);
	}
private:
    LuaBoolean(const bool value) : value(value) {}
};

class LuaNumber final {
public:
    const LuaType type = LuaType::kNumber;
    const double value;
    static LuaNumber make(const double value) {
		return LuaNumber(value);
	}
private:
    LuaNumber(const double value) : value(value) {}
};

class LuaString final {
public:
    const LuaType type = LuaType::kString;
    const std::string value;
    static LuaString make(const std::string& value) {
		return LuaString(value);
	}
private:
    LuaString(const std::string& value) : value(value) {}
};

class LuaFunction final {
public:
    const LuaType type = LuaType::kFunction;
    const std::nullptr_t value = nullptr;
    static LuaFunction make() {
		return LuaFunction();
	}
private:
    LuaFunction() = default;
};

class LuaTable final {
public:
    const LuaType type = LuaType::kTable;
    const std::nullptr_t value = nullptr;
    static LuaTable make() {
		return LuaTable();
	}
private:
    LuaTable() = default;
};

class LuaLightUserData final {
public:
    const LuaType type = LuaType::kLightUserData;
    void* value;
    static LuaLightUserData make(void* value) {
		return LuaLightUserData(value);
	}
private:
    LuaLightUserData(void* value) : value(value) {}
};

using LuaValue = std::variant<LuaNil, LuaBoolean, LuaNumber, LuaString, LuaFunction, LuaTable, LuaLightUserData>;

inline LuaType get_lua_type(const LuaValue& value) {
    return std::visit([](const auto &v) {
		return v.type;
	}, value);
}

inline std::string to_string(const LuaValue& value) {
    switch (get_lua_type(value)) {
    	case LuaType::kNil:
    	    return "nil";
    	case LuaType::kBoolean:
    	    return std::get<LuaBoolean>(value).value ? "true" : "false";
    	case LuaType::kNumber:
    	    return std::to_string(std::get<LuaNumber>(value).value);
    	case LuaType::kString:
    	    return std::get<LuaString>(value).value;
    	case LuaType::kFunction:
    	    return "function";
    	case LuaType::kTable:
    	    return "table";
    	case LuaType::kLightUserData:
    	    return "lightuserdata";
		default:
			return "Unknown type";
    }
}

} // namespace cclua

#endif // LUA_TYPE_H
