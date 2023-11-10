#include "gd_equaio.h"
#include "gd_extension_utils.h"
#include <utils.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// ======================= CONTEXT
void EqContext::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set", "variables", "binary_operators", "unary_operators", "handle_numerics"), 
                         &EqContext::set);
    ClassDB::bind_method(D_METHOD("get"), &EqContext::get);
}

EqContext::EqContext()  { this->ctx = {}; }
EqContext::~EqContext() { }

void EqContext::set(
    TypedArray<String> variables, TypedArray<String> binary_operators,
    TypedArray<String> unary_operators, bool handle_numerics
){
    this->ctx.variables = from_godot(variables);
    this->ctx.binary_operators = from_godot(binary_operators);
    this->ctx.unary_operators = from_godot(unary_operators);
    this->ctx.handle_numerics = handle_numerics;
}

Array EqContext::get(){
    Array arr;
    TypedArray<String> variables = to_godot(this->ctx.variables);
    TypedArray<String> binary_operators = to_godot(this->ctx.binary_operators);
    TypedArray<String> unary_operators = to_godot(this->ctx.unary_operators);
    arr.push_back(variables);
    arr.push_back(binary_operators);
    arr.push_back(unary_operators);
    arr.push_back(this->ctx.handle_numerics);
    return arr;
}


// ======================= EXPRESSION
void EqExpression::_bind_methods() {
    ClassDB::bind_method(D_METHOD("to_string"), &EqExpression::to_string);
    ClassDB::bind_method(D_METHOD("copy", "destination"), &EqExpression::copy);
    ClassDB::bind_method(D_METHOD("parse", "expression", "context"), &EqExpression::parse);
}
//
EqExpression::EqExpression()  { this->expr = {}; }
EqExpression::~EqExpression() { }
String EqExpression::to_string() { return to_godot(this->expr.to_string()); }
void EqExpression::copy(EqExpression* dst) { dst->expr = this->expr.copy(); }
void EqExpression::parse(String strexpr, EqContext *ctx) {
    auto expr = parse_expression(from_godot(strexpr), ctx->ctx);
    if(expr.has_value()) this->expr = expr.value();
}

// ======================== BLOCK DISPLAY
void EqBlockDisplay::_bind_methods() {
    ClassDB::bind_method(D_METHOD("to_string"), &EqBlockDisplay::to_string);
    ClassDB::bind_method(D_METHOD("append", "block"), &EqBlockDisplay::append);
    ClassDB::bind_method(D_METHOD("prepend", "block"), &EqBlockDisplay::prepend);
    ClassDB::bind_method(D_METHOD("append_arr", "blocks"), &EqBlockDisplay::append_arr);
    ClassDB::bind_method(D_METHOD("prepend_arr", "blocks"), &EqBlockDisplay::prepend_arr);
    ClassDB::bind_method(D_METHOD("from_expression", "expression", "context"), &EqBlockDisplay::from_expression);
    ClassDB::bind_method(D_METHOD("get_type"), &EqBlockDisplay::get_type);
    ClassDB::bind_method(D_METHOD("get_value"), &EqBlockDisplay::get_value);
    ClassDB::bind_method(D_METHOD("get_children_count"), &EqBlockDisplay::get_children_count);
    ClassDB::bind_method(D_METHOD("get_child_at", "index", "destination"), &EqBlockDisplay::get_child_at);
    // ClassDB::bind_method(D_METHOD("get_child"), &EqBlockDisplay::get_child);
    ClassDB::bind_method(D_METHOD("get_metadata_address"), &EqBlockDisplay::get_metadata_address);
    ClassDB::bind_method(D_METHOD("get_metadata_leftaddr"), &EqBlockDisplay::get_metadata_leftaddr);
    ClassDB::bind_method(D_METHOD("get_metadata_rightaddr"), &EqBlockDisplay::get_metadata_rightaddr);
    ClassDB::bind_method(D_METHOD("get_metadata_expression", "destination"), &EqBlockDisplay::get_metadata_expression);
}

EqBlockDisplay::EqBlockDisplay()  { this->block = {}; }
EqBlockDisplay::~EqBlockDisplay() { }
void EqBlockDisplay::append(EqBlockDisplay* b){
    this->block.append(b->block);
}
void EqBlockDisplay::append_arr(TypedArray<EqBlockDisplay> bs){
    for(int i = 0; i < bs.size(); i++){
        EqBlockDisplay* b = Object::cast_to<EqBlockDisplay>(bs[i]);
        this->append(b);
    } 
}
void EqBlockDisplay::prepend(EqBlockDisplay* b){
    this->block.prepend(b->block);
}
void EqBlockDisplay::prepend_arr(TypedArray<EqBlockDisplay> bs){
    for(int i = 0; i < bs.size(); i++){
        EqBlockDisplay* b = Object::cast_to<EqBlockDisplay>(bs[i]);
        this->prepend(b);
    }
}
String EqBlockDisplay::to_string() const{
    return to_godot(this->block.to_string());
}
void EqBlockDisplay::from_expression(EqExpression* expr, EqContext* ctx){
    BlockDisplay::Block block = BlockDisplay::from_expression(expr->expr, ctx->ctx);
    this->block = block;
}

int EqBlockDisplay::get_type() const{ return this->block.type; }
String EqBlockDisplay::get_value() const{ return to_godot(this->block.value); }
int EqBlockDisplay::get_children_count() const{ return this->block.children.size(); }
void EqBlockDisplay::get_child_at(int id, EqBlockDisplay* dst) const{
    dst->block = this->block.children[id];
}
// TypedArray<EqBlockDisplay> EqBlockDisplay::get_children() const{
//     TypedArray<EqBlockDisplay> arr;
//     arr.resize(this->block.children.size());
//     for(int i = 0; i < this->block.children.size(); i++){
//         EqBlockDisplay *b = new EqBlockDisplay();
//         b->block = this->block.children[i];
//         arr[i] = b;
//     }
//     return arr;
// }
TypedArray<int> EqBlockDisplay::get_metadata_address() const{ return to_godot(this->block.metadata.addr); }
TypedArray<int> EqBlockDisplay::get_metadata_leftaddr() const{ return to_godot(this->block.metadata.leftaddr); }
TypedArray<int> EqBlockDisplay::get_metadata_rightaddr() const{ return to_godot(this->block.metadata.rightaddr); }
void EqBlockDisplay::get_metadata_expression(EqExpression* dst) const{
    dst->expr = *this->block.metadata.expr;
}


// ======================= TASK
void EqTask::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_context", "destination"), &EqTask::get_context);
    ClassDB::bind_method(D_METHOD("get_rules_names"), &EqTask::get_rules_names);
    ClassDB::bind_method(D_METHOD("get_rule_expr", "name", "destination"), &EqTask::get_rule_expr);
    ClassDB::bind_method(D_METHOD("get_target", "destination"), &EqTask::get_target);
    ClassDB::bind_method(D_METHOD("get_current", "destination"), &EqTask::get_current);
    ClassDB::bind_method(D_METHOD("get_print_rhs_only"), &EqTask::get_print_rhs_only);
    ClassDB::bind_method(D_METHOD("get_history_count"), &EqTask::get_history_count);
    ClassDB::bind_method(D_METHOD("get_history_expression_at", "id", "destination"), 
                         &EqTask::get_history_expression_at);
    // ClassDB::bind_method(D_METHOD("get_history_message_at", "id"), &EqTask::get_history_message_at);
    ClassDB::bind_method(D_METHOD("get_error_messages_count"), &EqTask::get_error_messages_count);
    ClassDB::bind_method(D_METHOD("get_error_message_at", "id"), &EqTask::get_error_message_at);
    ClassDB::bind_method(D_METHOD("set_print_rhs_only", "value"), &EqTask::set_print_rhs_only);

    ClassDB::bind_method(D_METHOD("add_rule_expr", "name", "expr"), &EqTask::add_rule_expr);
    ClassDB::bind_method(D_METHOD("add_rule_eq", "name", "expr"), &EqTask::add_rule_eq);
    ClassDB::bind_method(D_METHOD("set_target_eq", "expr"), &EqTask::set_target_eq);
    ClassDB::bind_method(D_METHOD("set_current_eq", "expr"), &EqTask::set_current_eq);
    ClassDB::bind_method(D_METHOD("init_current_with_target_lhs"), &EqTask::init_current_with_target_lhs);
    ClassDB::bind_method(D_METHOD("set_current_expr", "expr", "msg"), &EqTask::set_current_expr);
    ClassDB::bind_method(D_METHOD("apply_function_to_both_side_expr", "fexpr", "varname"), 
                         &EqTask::apply_function_to_both_side_expr);
    ClassDB::bind_method(D_METHOD("apply_rule_expr", "expr"), &EqTask::apply_rule_expr);
    ClassDB::bind_method(D_METHOD("apply_function_to_both_side", "fstr", "varname"), &EqTask::apply_function_to_both_side);
    ClassDB::bind_method(D_METHOD("apply_rule", "rulename"), &EqTask::apply_rule);
    ClassDB::bind_method(D_METHOD("try_swap_two_element", "addr1", "addr2"), &EqTask::try_swap_two_element);
    ClassDB::bind_method(D_METHOD("apply_arithmetic_to_both_side", "op", "value"), &EqTask::apply_arithmetic_to_both_side);
    ClassDB::bind_method(D_METHOD("apply_arithmetic_calculation", "left", "right", "op"), 
                         &EqTask::apply_arithmetic_calculation);
    ClassDB::bind_method(D_METHOD("apply_arithmetic_turn_subtraction_to_addition"), 
                         &EqTask::apply_arithmetic_turn_subtraction_to_addition);
    ClassDB::bind_method(D_METHOD("apply_arithmetic_turn_addition_to_subtraction"), 
                         &EqTask::apply_arithmetic_turn_addition_to_subtraction);
    ClassDB::bind_method(D_METHOD("apply_arithmetic_turn_division_to_multiplication"), 
                         &EqTask::apply_arithmetic_turn_division_to_multiplication);
    ClassDB::bind_method(D_METHOD("apply_arithmetic_turn_multiplication_to_division"), 
                         &EqTask::apply_arithmetic_turn_multiplication_to_division);
}
EqTask::EqTask()  { this->task = {}; }
EqTask::~EqTask() { }

// getter
void EqTask::get_context(EqContext* dst) const { dst->ctx = this->task.context; }
TypedArray<String> EqTask::get_rules_names() const{
    TypedArray<String> arr;
    for(auto [name, _] : this->task.rules){ arr.push_back(to_godot(name)); }
    return arr;
}
void EqTask::get_rule_expr(String name, EqExpression* dst) const{
    if(map_contain(from_godot(name), this->task.rules)) dst->expr = this->task.rules.at(from_godot(name));
}
void EqTask::get_target(EqExpression* dst) const{
    if (this->task.target.has_value()) dst->expr = this->task.target.value();
}
void EqTask::get_current(EqExpression* dst) const{
    if (this->task.current.has_value()) dst->expr = this->task.current.value();
}
bool EqTask::get_print_rhs_only() const{ return this->task.print_rhs_only; }
int EqTask::get_history_count() const{ return this->task.history.size(); }
void EqTask::get_history_expression_at(int id, EqExpression* dst) const {
    if (id < this->task.history.size()){
        auto [expr,_]  = this->task.history[id];
        dst->expr = expr;
    }
}
String EqTask::get_history_message_at(int id) const{
    if (id < this->task.history.size()){
        auto [_,msg]  = this->task.history[id];
        return to_godot(msg);
    }
    return to_godot("");
}
int EqTask::get_error_messages_count() const{
    return this->task.error_messages.size();
}
String EqTask::get_error_message_at(int id) const{
    if (id < this->task.error_messages.size()){
        return to_godot(this->task.error_messages[id]);
    }
    return to_godot("");
}

void EqTask::set_print_rhs_only(bool value){ this->task.print_rhs_only = value; }


// inits
void EqTask::add_rule_expr(String name, EqExpression* expr){}
void EqTask::add_rule_eq(String name, String exprstr){}

// return false if failed
bool EqTask::set_target_eq(String exprstr){ return this->task.set_target_eq(from_godot(exprstr)); }
bool EqTask::set_current_eq(String exprstr){ return this->task.set_current_eq(from_godot(exprstr)); }
void EqTask::init_current_with_target_lhs(){ this->task.init_current_with_target_lhs(); }
void EqTask::set_current_expr(EqExpression* expr, String msg){ this->task.set_current_expr(expr->expr, from_godot(msg)); }

// userland
bool EqTask::apply_function_to_both_side_expr(EqExpression* fexpr, String varname){
    return this->task.apply_function_to_both_side_expr(fexpr->expr, from_godot(varname));
}
bool EqTask::apply_rule_expr(EqExpression* expr){
    return this->task.apply_rule_expr(expr->expr);
}
bool EqTask::apply_function_to_both_side(String fstr, String varname){
    return this->task.apply_function_to_both_side(from_godot(fstr), from_godot(varname));
}
bool EqTask::apply_rule(String rulename){
    return this->task.apply_rule(from_godot(rulename));
}
bool EqTask::try_swap_two_element(EqAddress addr1, EqAddress addr2){
    return this->task.try_swap_two_element(from_godot(addr1), from_godot(addr2));
}

// arithmetics
bool EqTask::apply_arithmetic_to_both_side(int op, String value){
    return this->task.apply_arithmetic_to_both_side((Arithmetic::Operator)op, from_godot(value));
}
bool EqTask::apply_arithmetic_calculation(String left, String right, int op){
    return this->task.apply_arithmetic_calculation(from_godot(left), from_godot(right), 
                                                   (Arithmetic::Operator)op);
}
bool EqTask::apply_arithmetic_turn_subtraction_to_addition(){
    return this->task.apply_arithmetic_turn_subtraction_to_addition();
}
bool EqTask::apply_arithmetic_turn_addition_to_subtraction(){
    return this->task.apply_arithmetic_turn_addition_to_subtraction();
}
bool EqTask::apply_arithmetic_turn_division_to_multiplication(){
    return this->task.apply_arithmetic_turn_division_to_multiplication();
}
bool EqTask::apply_arithmetic_turn_multiplication_to_division(){
    return this->task.apply_arithmetic_turn_multiplication_to_division();
}
