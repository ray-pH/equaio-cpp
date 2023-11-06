#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/node.hpp>
#include <libequaio.h>
#include <block_display.h>
#include <parser.h>

namespace godot {

//------------------------------
class EqContext : public Object {
    GDCLASS(EqContext, Object);
private:
protected:
    static void _bind_methods();
public:
    Context ctx;
    EqContext();
    ~EqContext();
    void set(
        TypedArray<String> variables,
        TypedArray<String> binary_operators,
        TypedArray<String> unary_operators,
        bool handle_numerics
    );
    Array get();
};

//------------------------------
class EqExpression : public Object {
    GDCLASS(EqExpression, Object);
private:
    Expression expr;
protected:
    static void _bind_methods();
public:
    EqExpression();
    ~EqExpression();
    EqExpression* copy();
    String to_string();
    void parse(String strexpr, EqContext* ctx);
    // void parse(String strexpr);
};

}

#endif
