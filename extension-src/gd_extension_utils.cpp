#include "gd_extension_utils.h"

godot::String to_godot(const std::string &s) { return godot::String(s.c_str()); }
std::string from_godot(const godot::String &s) { return s.utf8().get_data(); }

godot::TypedArray<godot::String> to_godot(const std::vector<std::string> &v) {
    godot::TypedArray<godot::String> result;
    result.resize(v.size());
    for (int i = 0; i < v.size(); i++) result[i] = to_godot(v[i]);
    return result;
}
std::vector<std::string> from_godot(const godot::TypedArray<godot::String> &v) {
    std::vector<std::string> result;
    result.resize(v.size());
    for (int i = 0; i < v.size(); i++) result[i] = from_godot((godot::String)v[i]);
    return result;
}

godot::TypedArray<int> to_godot(const std::vector<int> &v) {
    godot::TypedArray<int> result;
    result.resize(v.size());
    for (int i = 0; i < v.size(); i++) result[i] = v[i];
    return result;
}
std::vector<int> from_godot(const godot::TypedArray<int> &v) {
    std::vector<int> result;
    result.resize(v.size());
    for (int i = 0; i < v.size(); i++) result[i] = v[i];
    return result;
}
