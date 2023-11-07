#include "gd_equaio.h"
#include "gd_extension_utils.h"
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
    ClassDB::bind_method(D_METHOD("copy"), &EqExpression::copy);
    ClassDB::bind_method(D_METHOD("parse", "expression", "context"), &EqExpression::parse);
}
//
EqExpression::EqExpression()  { this->expr = {}; }
EqExpression::~EqExpression() { }
String EqExpression::to_string() { return to_godot(this->expr.to_string()); }
EqExpression* EqExpression::copy() {
    EqExpression *expr = new EqExpression();
    expr->expr = this->expr;
    return expr;
}
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
    ClassDB::bind_method(D_METHOD("get_child_count"), &EqBlockDisplay::get_child_count);
    ClassDB::bind_method(D_METHOD("get_child_at", "index", "destination"), &EqBlockDisplay::get_child_at);
    ClassDB::bind_method(D_METHOD("get_child"), &EqBlockDisplay::get_child);
    ClassDB::bind_method(D_METHOD("get_metadata_address"), &EqBlockDisplay::get_metadata_address);
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
int EqBlockDisplay::get_child_count() const{ return this->block.child.size(); }
void EqBlockDisplay::get_child_at(int id, EqBlockDisplay* dst) const{
    dst->block = this->block.child[id];
}
TypedArray<EqBlockDisplay> EqBlockDisplay::get_child() const{
    TypedArray<EqBlockDisplay> arr;
    arr.resize(this->block.child.size());
    for(int i = 0; i < this->block.child.size(); i++){
        EqBlockDisplay *b = new EqBlockDisplay();
        b->block = this->block.child[i];
        arr[i] = b;
    }
    return arr;
}
TypedArray<int> EqBlockDisplay::get_metadata_address() const{
    return to_godot(this->block.metadata.addr);
}
void EqBlockDisplay::get_metadata_expression(EqExpression* dst) const{
    dst->expr = *this->block.metadata.expr;
}
