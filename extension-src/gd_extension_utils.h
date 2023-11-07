#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

godot::String to_godot(const std::string &s);
std::string from_godot(const godot::String &s);

godot::TypedArray<godot::String> to_godot(const std::vector<std::string> &v);
std::vector<std::string> from_godot(const godot::TypedArray<godot::String> &v);

godot::TypedArray<int> to_godot(const std::vector<int> &v);
std::vector<int> from_godot(const godot::TypedArray<int> &v);
