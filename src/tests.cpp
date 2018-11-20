#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "sass.h"

// NOTE: this probably leaks memory here and there
// this is only meant to test some libsass interna

// most functions are very simple
#define IMPLEMENT_IS_FN(fn) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  const union Sass_Value* inp = sass_list_get_value(s_args, 0); \
  bool is = sass_##fn(inp); \
  return sass_make_boolean(is); \
} \

// type test functions
IMPLEMENT_IS_FN(value_is_null)
IMPLEMENT_IS_FN(value_is_number)
IMPLEMENT_IS_FN(value_is_string)
IMPLEMENT_IS_FN(value_is_boolean)
IMPLEMENT_IS_FN(value_is_color)
IMPLEMENT_IS_FN(value_is_list)
IMPLEMENT_IS_FN(value_is_map)
IMPLEMENT_IS_FN(value_is_error)
IMPLEMENT_IS_FN(value_is_warning)

// most functions are very simple
#define IMPLEMENT_GET_NR(fn, type, rv_type, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  const union Sass_Value* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type(inp)) { \
    return sass_make_error("You must pass a " #type " into " #fn); \
  } \
  double rv = sass_##fn(inp); \
  return sass_make_number(rv, ""); \
} \

// most functions are very simple
#define IMPLEMENT_GET_BOOL(fn, type, rv_type, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  const union Sass_Value* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type(inp)) { \
    return sass_make_error("You must pass a " #type " into " #fn); \
  } \
  bool rv = sass_##fn(inp); \
  return sass_make_##rv_type(rv); \
} \

// most functions are very simple
#define IMPLEMENT_GET_STR(fn, type, rv_type, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  const union Sass_Value* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type(inp)) { \
    return sass_make_error("You must pass a " #type " into " #fn); \
  } \
  const char* rv = sass_##fn(inp); \
  return sass_make_string(rv); \
} \

// most functions are very simple
#define IMPLEMENT_SET_FN(fn, type1, rv_type, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 2) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  union Sass_Value* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(inp)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  union Sass_Value* arg = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_##rv_type(arg)) { \
    return sass_make_error("You must pass a " #rv_type " into " #fn); \
  } \
  sass_##fn(inp, sass_##rv_type##_get_##val_type(arg)); \
  return sass_clone_value(inp); \
} \


// most functions are very simple
#define IMPLEMENT_SET_STR(fn, type1, rv_type, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 2) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  union Sass_Value* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(inp)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  union Sass_Value* arg = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_string(arg)) { \
    return sass_make_error("You must pass a string into " #fn); \
  } \
  sass_##fn(inp, strdup(sass_string_get_value(arg))); \
  return sass_clone_value(inp); \
} \

// number functions
IMPLEMENT_GET_NR(number_get_value, number, number, value)
IMPLEMENT_SET_FN(number_set_value, number, number, value)
IMPLEMENT_GET_STR(number_get_unit, number, number, unit)
IMPLEMENT_SET_STR(number_set_unit, number, number, unit)

// string functions
IMPLEMENT_GET_STR(string_get_value, string, string, value)
IMPLEMENT_SET_STR(string_set_value, string, string, value)
IMPLEMENT_IS_FN(string_is_quoted)

// boolean functions
IMPLEMENT_GET_BOOL(boolean_get_value, boolean, boolean, value)
IMPLEMENT_SET_FN(boolean_set_value, boolean, boolean, value)

// color functions
IMPLEMENT_GET_NR(color_get_r, color, number, value)
IMPLEMENT_SET_FN(color_set_r, color, number, value)
IMPLEMENT_GET_NR(color_get_g, color, number, value)
IMPLEMENT_SET_FN(color_set_g, color, number, value)
IMPLEMENT_GET_NR(color_get_b, color, number, value)
IMPLEMENT_SET_FN(color_set_b, color, number, value)
IMPLEMENT_GET_NR(color_get_a, color, number, value)
IMPLEMENT_SET_FN(color_set_a, color, number, value)


// most functions are very simple
#define IMPLEMENT_GET_ITEM(fn, type1, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 2) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  union Sass_Value* cont = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(cont)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  union Sass_Value* item = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_number(item)) { \
    return sass_make_error("You must pass a number into " #fn); \
  } \
  union Sass_Value* value = sass_##fn(cont, sass_number_get_value(item)); \
  return sass_clone_value(value); \
} \

// most functions are very simple
#define IMPLEMENT_SET_ITEM(fn, type1, val_type) \
union Sass_Value* fn_##fn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_length(s_args) != 3) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  union Sass_Value* cont = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(cont)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  union Sass_Value* item = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_number(item)) { \
    return sass_make_error("You must pass a number into " #fn); \
  } \
  union Sass_Value* val = sass_list_get_value(s_args, 2); \
  val = sass_clone_value(val); \
  sass_##fn(cont, sass_number_get_value(item), val); \
  return sass_clone_value(cont); \
} \

// list functions
IMPLEMENT_GET_NR(list_get_length, list, number, value)
IMPLEMENT_GET_NR(list_get_separator, list, number, value)
// IMPLEMENT_SET_FN(list_set_separator, number, number, value)
IMPLEMENT_GET_ITEM(list_get_value, list, value)
IMPLEMENT_SET_ITEM(list_set_value, list, value)

// map functions
IMPLEMENT_GET_NR(map_get_length, map, number, value)
IMPLEMENT_GET_ITEM(map_get_key, map, key)
IMPLEMENT_SET_ITEM(map_set_key, map, key)
IMPLEMENT_GET_ITEM(map_get_value, map, value)
IMPLEMENT_SET_ITEM(map_set_value, map, value)

union Sass_Value* fn_warn(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp)
{
  // Tests sass_make_warning, sass_warning_get_message, and sass_warning_set_message.
  union Sass_Value* msg = sass_make_warning("warning");
  sass_warning_set_message(msg, sass_warning_get_message(msg));
  std::free(sass_warning_get_message(msg));
  sass_warning_set_message(msg, strdup("warning caught"));

  Sass_Callee_Entry callee = sass_compiler_get_last_callee(comp);
  Sass_Env_Frame env = sass_callee_get_env(callee);
  union Sass_Value* val = sass_clone_value(s_args);
  sass_env_set_lexical(env, "$warn", val);
  sass_delete_value(val);
  union Sass_Value* result = sass_value_stringify(msg, false, 5);
  sass_delete_value(msg);
  return result;
}

union Sass_Value* fn_debug(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp)
{
  // union Sass_Value* msg = sass_make_warning("debug caught");
  Sass_Callee_Entry callee = sass_compiler_get_last_callee(comp);
  Sass_Env_Frame env = sass_callee_get_env(callee);
  union Sass_Value* val = sass_clone_value(s_args);
  sass_env_set_lexical(env, "$debug", val);
  union Sass_Value* result = sass_value_stringify(val, false, 5);
  sass_delete_value(val);
  return result;
}

union Sass_Value* fn_error(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp)
{
  // Tests sass_make_error, sass_error_get_message, and sass_error_set_message.
  union Sass_Value* msg = sass_make_error("error");
  sass_error_set_message(msg, sass_error_get_message(msg));
  std::free(sass_error_get_message(msg));
  sass_error_set_message(msg, strdup("error caught"));

  Sass_Callee_Entry callee = sass_compiler_get_last_callee(comp);
  Sass_Env_Frame env = sass_callee_get_env(callee);
  union Sass_Value* val = sass_clone_value(s_args);
  sass_env_set_lexical(env, "$error", val);
  sass_delete_value(val);
  union Sass_Value* result = sass_value_stringify(msg, false, 5);
  sass_delete_value(msg);
  return result;
}

// specific implementation of sass value ops
union Sass_Value* fn_value_op(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp)
{
  if (!sass_value_is_list(s_args)) {
    return sass_make_error("Invalid arguments for sass_op");
  }
  if (sass_list_get_length(s_args) != 3) {
    return sass_make_error("Exactly three arguments expected for sass_op");
  }
  union Sass_Value* cmd = sass_list_get_value(s_args, 0);
  union Sass_Value* lhs = sass_list_get_value(s_args, 1);
  union Sass_Value* rhs = sass_list_get_value(s_args, 2);

  if (!sass_value_is_string(cmd)) {
    return sass_make_error("You must pass a string as operator");
  }
  std::string str(sass_string_get_value(cmd));
  enum Sass_OP op = ADD;
  if ("&&" == str) { op = AND; }
  else if ("||" == str) { op = OR; }
  else if ("==" == str) { op = EQ; }
  else if ("!=" == str) { op = NEQ; }
  else if (">=" == str) { op = GTE; }
  else if ("<=" == str) { op = LTE; }
  else if (">" == str) { op = GT; }
  else if ("<" == str) { op = LT; }
  else if ("+" == str) { op = ADD; }
  else if ("-" == str) { op = SUB; }
  else if ("*" == str) { op = MUL; }
  else if ("/" == str) { op = DIV; }
  else if ("%" == str) { op = MOD; }
  return sass_value_op(op, lhs, rhs);
}

// specific implementation of sass value ops
union Sass_Value* fn_value_stringify(const union Sass_Value* s_args, Sass_Function_Entry cb, struct Sass_Compiler* comp)
{
  if (!sass_value_is_list(s_args)) {
    return sass_make_error("Invalid arguments for sass_op");
  }
  if (sass_list_get_length(s_args) != 1) {
    return sass_make_error("Exactly three arguments expected for sass_op");
  }
  union Sass_Value* ex = sass_list_get_value(s_args, 0);
  return sass_value_stringify(ex, false, 5);
}

// return version of libsass we are linked against
extern "C" const char* ADDCALL libsass_get_version() {
  return libsass_version();
}

// entry point for libsass to request custom functions from plugin
extern "C" Sass_Function_List ADDCALL libsass_load_functions()
{

  // create list of all custom functions
  Sass_Function_List fn_list = sass_make_function_list(40);

  sass_function_set_list_entry(fn_list,  0, sass_make_function("sass_value_is_null($value)", fn_value_is_null, 0));
  sass_function_set_list_entry(fn_list,  1, sass_make_function("sass_value_is_number($value)", fn_value_is_number, 0));
  sass_function_set_list_entry(fn_list,  2, sass_make_function("sass_value_is_string($value)", fn_value_is_string, 0));
  sass_function_set_list_entry(fn_list,  3, sass_make_function("sass_value_is_boolean($value)", fn_value_is_boolean, 0));
  sass_function_set_list_entry(fn_list,  4, sass_make_function("sass_value_is_color($value)", fn_value_is_color, 0));
  sass_function_set_list_entry(fn_list,  5, sass_make_function("sass_value_is_list($value)", fn_value_is_list, 0));
  sass_function_set_list_entry(fn_list,  6, sass_make_function("sass_value_is_map($value)", fn_value_is_map, 0));
  sass_function_set_list_entry(fn_list,  7, sass_make_function("sass_value_is_error($value)", fn_value_is_error, 0));
  sass_function_set_list_entry(fn_list,  8, sass_make_function("sass_value_is_warning($value)", fn_value_is_warning, 0));

  sass_function_set_list_entry(fn_list,  9, sass_make_function("sass_number_get_value($number)", fn_number_get_value, 0));
  sass_function_set_list_entry(fn_list, 10, sass_make_function("sass_number_set_value($number, $value)", fn_number_set_value, 0));
  sass_function_set_list_entry(fn_list, 11, sass_make_function("sass_number_get_unit($number)", fn_number_get_unit, 0));
  sass_function_set_list_entry(fn_list, 12, sass_make_function("sass_number_set_unit($number, $unit)", fn_number_set_unit, 0));

  sass_function_set_list_entry(fn_list, 13, sass_make_function("sass_string_get_value($string)", fn_string_get_value, 0));
  sass_function_set_list_entry(fn_list, 14, sass_make_function("sass_string_set_value($string, $value)", fn_string_set_value, 0));

  sass_function_set_list_entry(fn_list, 15, sass_make_function("sass_boolean_get_value($bool)", fn_boolean_get_value, 0));
  sass_function_set_list_entry(fn_list, 16, sass_make_function("sass_boolean_set_value($bool, $value)", fn_boolean_set_value, 0));

  sass_function_set_list_entry(fn_list, 17, sass_make_function("sass_color_get_r($color)", fn_color_get_r, 0));
  sass_function_set_list_entry(fn_list, 18, sass_make_function("sass_color_get_g($color)", fn_color_get_g, 0));
  sass_function_set_list_entry(fn_list, 19, sass_make_function("sass_color_get_b($color)", fn_color_get_b, 0));
  sass_function_set_list_entry(fn_list, 20, sass_make_function("sass_color_get_a($color)", fn_color_get_a, 0));

  sass_function_set_list_entry(fn_list, 21, sass_make_function("sass_color_set_r($color, $nr)", fn_color_set_r, 0));
  sass_function_set_list_entry(fn_list, 22, sass_make_function("sass_color_set_g($color, $nr)", fn_color_set_g, 0));
  sass_function_set_list_entry(fn_list, 23, sass_make_function("sass_color_set_b($color, $nr)", fn_color_set_b, 0));
  sass_function_set_list_entry(fn_list, 24, sass_make_function("sass_color_set_a($color, $nr)", fn_color_set_a, 0));

  sass_function_set_list_entry(fn_list, 25, sass_make_function("@warn($warn)", fn_warn, 0));
  sass_function_set_list_entry(fn_list, 26, sass_make_function("@debug($debug)", fn_debug, 0));
  sass_function_set_list_entry(fn_list, 27, sass_make_function("@error($error)", fn_error, 0));

  sass_function_set_list_entry(fn_list, 28, sass_make_function("sass_string_is_quoted($string)", fn_string_is_quoted, 0));

  sass_function_set_list_entry(fn_list, 29, sass_make_function("sass_list_get_length($list)", fn_list_get_length, 0));
  sass_function_set_list_entry(fn_list, 30, sass_make_function("sass_list_get_separator($list)", fn_list_get_separator, 0));
  sass_function_set_list_entry(fn_list, 31, sass_make_function("sass_list_get_value($list, $i)", fn_list_get_value, 0));
  sass_function_set_list_entry(fn_list, 32, sass_make_function("sass_list_set_value($list, $i, $value)", fn_list_set_value, 0));

  sass_function_set_list_entry(fn_list, 33, sass_make_function("sass_map_get_length($list)", fn_map_get_length, 0));
  sass_function_set_list_entry(fn_list, 34, sass_make_function("sass_map_get_key($list, $i)", fn_map_get_key, 0));
  sass_function_set_list_entry(fn_list, 35, sass_make_function("sass_map_get_value($list, $i)", fn_map_get_value, 0));
  sass_function_set_list_entry(fn_list, 36, sass_make_function("sass_map_set_key($list, $i, $value)", fn_map_set_key, 0));
  sass_function_set_list_entry(fn_list, 37, sass_make_function("sass_map_set_value($list, $i, $value)", fn_map_set_value, 0));
  sass_function_set_list_entry(fn_list, 38, sass_make_function("sass_value_stringify($value)", fn_value_stringify, 0));
  sass_function_set_list_entry(fn_list, 39, sass_make_function("sass_value_op($op, $lhs, $rhs)", fn_value_op, 0));

  // return the list
  return fn_list;

}

// entry point for libsass to request custom headers from plugin
extern "C" Sass_Importer_List ADDCALL libsass_load_headers()
{
  // create list of all custom functions
  Sass_Importer_List imp_list = sass_make_importer_list(1);
  // put the only function in this plugin to the list
  sass_importer_set_list_entry(imp_list, 0, 0);
  // return the list
  return imp_list;
}
