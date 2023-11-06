#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/node.hpp>

namespace godot {

class GDEquaio : public Node {
    GDCLASS(GDEquaio, Node);
protected:
	static void _bind_methods();
public:
	GDEquaio();
	~GDEquaio();

	void _process(double delta) override;
    int factorial(int n);
    TypedArray<int> factorials(int n);
};

}

#endif
