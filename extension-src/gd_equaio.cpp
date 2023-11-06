#include "gd_equaio.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDEquaio::_bind_methods() {
    ClassDB::bind_method(D_METHOD("factorial", "number"), &GDEquaio::factorial);
	// ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &GDExample::set_amplitude);
}

GDEquaio::GDEquaio() {
	// Initialize any variables here.
}

GDEquaio::~GDEquaio() {
	// Add your cleanup here.
}

void GDEquaio::_process(double delta) {
}

int GDEquaio::factorial(int n){
    if (n <= 1) return 1;
    else return n * factorial(n-1);
}
TypedArray<int> GDEquaio::factorials(int n) {
    auto result = TypedArray<int>();
    for (int i = 0; i < n; i++) {
        result.push_back(this->factorial(i));
    }
    return result;
}
