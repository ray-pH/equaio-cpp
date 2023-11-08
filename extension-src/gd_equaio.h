#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/node.hpp>
#include <expression.h>
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
protected:
    static void _bind_methods();
public:
    Expression expr;

    EqExpression();
    ~EqExpression();
    void copy(EqExpression* dst);
    String to_string();
    void parse(String strexpr, EqContext* ctx);
    // void parse(String strexpr);
};

//-----------------------------
class EqBlockDisplay : public Object {
    GDCLASS(EqBlockDisplay, Object);
private:
    BlockDisplay::Block block;
protected:
    static void _bind_methods();
public:
    EqBlockDisplay();
    ~EqBlockDisplay();

    void append(EqBlockDisplay* b);
    void append_arr(TypedArray<EqBlockDisplay> bs);
    void prepend(EqBlockDisplay* b);
    void prepend_arr(TypedArray<EqBlockDisplay> bs);
    String to_string() const;
    void from_expression(EqExpression* expr, EqContext* ctx);

    int get_type() const;
    String get_value() const;
    int get_children_count() const;
    // EqBlockDisplay* get_child_at(int id) const;
    // we have to do this because we can't return a pointer to a custom object
    void get_child_at(int id, EqBlockDisplay* dst) const;
    // TypedArray<EqBlockDisplay> get_children() const;
    
    TypedArray<int> get_metadata_address() const;
    TypedArray<int> get_metadata_leftaddr() const;
    TypedArray<int> get_metadata_rightaddr() const;
    // EqExpression* get_metadata_expression() const;
    void get_metadata_expression(EqExpression* dst) const;
};

}

#endif
