#include "gd_extension_utils.h"

godot::String to_godot(const std::string &s) { return godot::String(s.c_str()); }
std::string from_godot(const godot::String &s) { return s.utf8().get_data(); }

godot::TypedArray<godot::String> to_godot(const std::vector<std::string> &v) {
    godot::TypedArray<godot::String> result;
    for (const auto &s : v) result.push_back(to_godot(s));
    return result;
}
std::vector<std::string> from_godot(const godot::TypedArray<godot::String> &v) {
    std::vector<std::string> result;
    for (int i = 0; i < v.size(); i++) result.push_back(from_godot((godot::String)v[i]));
    return result;
}
