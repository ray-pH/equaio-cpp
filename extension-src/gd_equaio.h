#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/node.hpp>
#include <expression.h>
#include <block_display.h>
#include <parser.h>
#include <interface.h>

namespace godot {

typedef TypedArray<int> EqAddress;

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
    
    EqAddress get_metadata_address() const;
    EqAddress get_metadata_leftaddr() const;
    EqAddress get_metadata_rightaddr() const;
    // EqExpression* get_metadata_expression() const;
    void get_metadata_expression(EqExpression* dst) const;
};

//-----------------------------
class EqTask : public Object {
    GDCLASS(EqTask, Object);
private:
    Task task;
protected:
    static void _bind_methods();
public:
    EqTask();
    ~EqTask();

    // getter
    void get_context(EqContext* dst) const;
    TypedArray<String> get_rules_names() const;
    void get_rule_expr(String name, EqExpression* dst) const;
    void get_target(EqExpression* dst) const;
    void get_current(EqExpression* dst) const;
    bool get_print_rhs_only() const;
    int get_history_count() const;
    void get_history_expression_at(int id, EqExpression* dst) const;
    String get_history_message_at(int id) const;
    int get_error_messages_count() const;
    String get_error_message_at(int id) const;

    void set_print_rhs_only(bool value);

    // initialization
    void add_rule_expr(String name, EqExpression* expr);
    void add_rule_eq(String name, String exprstr);
    // return false if failed
    bool set_target_eq(String exprstr);
    bool set_current_eq(String exprstr);
    void init_current_with_target_lhs();
    void set_current_expr(EqExpression* expr, String msg);
    // userland

    bool apply_function_to_both_side_expr(EqExpression* fexpr, String varname);
    bool apply_rule_expr(EqExpression* expr);
    bool apply_function_to_both_side(String fstr, String varname);
    bool apply_rule(String rulename);
    bool try_swap_two_element(EqAddress addr1, EqAddress addr2);

    // arithmetics
    bool apply_arithmetic_to_both_side(int op, String value);
    bool apply_arithmetic_calculation(String left, String right, int op);
    bool apply_arithmetic_turn_subtraction_to_addition();
    bool apply_arithmetic_turn_addition_to_subtraction();
    bool apply_arithmetic_turn_division_to_multiplication();
    bool apply_arithmetic_turn_multiplication_to_division();
};
    

}

#endif
