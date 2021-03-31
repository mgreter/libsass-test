#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include "sass.h"

// most functions are very simple
#define IMPLEMENT_IS_FN(fn) \
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  struct SassValue* inp = sass_list_get_value(s_args, 0); \
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
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  struct SassValue* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type(inp)) { \
    return sass_make_error("You must pass a " #type " into " #fn); \
  } \
  double rv = sass_##fn(inp); \
  return sass_make_number(rv, ""); \
} \

// most functions are very simple
#define IMPLEMENT_GET_BOOL(fn, type, rv_type, val_type) \
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  struct SassValue* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type(inp)) { \
    return sass_make_error("You must pass a " #type " into " #fn); \
  } \
  bool rv = sass_##fn(inp); \
  return sass_make_##rv_type(rv); \
} \

// most functions are very simple
#define IMPLEMENT_GET_STR(fn, type, rv_type, val_type) \
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 1) { \
    return sass_make_error("Exactly one arguments expected for " #fn); \
  } \
  struct SassValue* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type(inp)) { \
    return sass_make_error("You must pass a " #type " into " #fn); \
  } \
  const char* rv = sass_##fn(inp); \
  return sass_make_string(rv, false); \
} \

// most functions are very simple
#define IMPLEMENT_SET_FN(fn, type1, rv_type, val_type) \
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 2) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  struct SassValue* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(inp)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  struct SassValue* arg = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_##rv_type(arg)) { \
    return sass_make_error("You must pass a " #rv_type " into " #fn); \
  } \
  sass_##fn(inp, sass_##rv_type##_get_##val_type(arg)); \
  return sass_clone_value(inp); \
} \


// most functions are very simple
#define IMPLEMENT_SET_STR(fn, get_fn, type1, rv_type, val_type) \
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 2) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  struct SassValue* inp = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(inp)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  struct SassValue* arg = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_string(arg)) { \
    return sass_make_error("You must pass a string into " #fn); \
  } \
  struct SassValue* result = sass_clone_value(inp); \
  sass_##fn(result, strdup(sass_string_get_value(arg))); \
  return result; \
} \

struct SassValue* fn_number_set_unit(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  struct SassValue* inp = sass_list_get_value(s_args, 0);
  struct SassValue* arg = sass_list_get_value(s_args, 1);
  struct SassValue* result = sass_clone_value(inp);
  sass_number_set_unit(result, sass_string_get_value(arg));
  return result;
}

struct SassValue* fn_sass_number_normalize(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  struct SassValue* inp = sass_list_get_value(s_args, 0);
  struct SassValue* result = sass_clone_value(inp);
  sass_number_normalize(result);
  return result;
}

struct SassValue* fn_sass_number_reduce(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  struct SassValue* inp = sass_list_get_value(s_args, 0);
  struct SassValue* result = sass_clone_value(inp);
  sass_number_reduce(result);
  return result;
}

// number functions
IMPLEMENT_GET_NR(number_get_value, number, number, value)
IMPLEMENT_SET_FN(number_set_value, number, number, value)
IMPLEMENT_GET_STR(number_get_unit, number, number, unit)
// IMPLEMENT_SET_STR(number_set_unit, number_get_unit, number, number, unit)

// string functions
IMPLEMENT_GET_STR(string_get_value, string, string, value)
IMPLEMENT_SET_STR(string_set_value, string_get_value, string, string, value)
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
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 2) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  struct SassValue* cont = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(cont)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  struct SassValue* item = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_number(item)) { \
    return sass_make_error("You must pass a number into " #fn); \
  } \
  struct SassValue* value = sass_##fn(cont, sass_number_get_value(item)); \
  return sass_clone_value(value); \
} \

// most functions are very simple
#define IMPLEMENT_SET_ITEM(fn, get_fn, type1, val_type) \
struct SassValue* fn_##fn(struct SassValue* s_args, struct SassCompiler* comp, void*) \
{ \
  if (!sass_value_is_list(s_args)) { \
    return sass_make_error("Invalid arguments for " #fn); \
  } \
  if (sass_list_get_size(s_args) != 3) { \
    return sass_make_error("Exactly two arguments expected for " #fn); \
  } \
  struct SassValue* cont = sass_list_get_value(s_args, 0); \
  if (!sass_value_is_##type1(cont)) { \
    return sass_make_error("You must pass a " #type1 " into " #fn); \
  } \
  struct SassValue* item = sass_list_get_value(s_args, 1); \
  if (!sass_value_is_number(item)) { \
    return sass_make_error("You must pass a number into " #fn); \
  } \
  struct SassValue* result = sass_clone_value(cont); \
  sass_delete_value(sass_##get_fn(result, sass_number_get_value(item))); \
  sass_##fn(result, sass_number_get_value(item), sass_clone_value(sass_list_get_value(s_args, 2))); \
  return result; \
} \

// list functions
IMPLEMENT_GET_NR(list_get_size, list, number, value)
IMPLEMENT_GET_NR(list_get_separator, list, number, value)
// IMPLEMENT_SET_FN(list_set_separator, number, number, value)
IMPLEMENT_GET_ITEM(list_get_value, list, value)
IMPLEMENT_SET_ITEM(list_set_value, list_get_value, list, value)

struct SassValue* fn_sass_map_make_iterator(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  SassValue* result = sass_make_list(SASS_COMMA, false);
  struct SassValue* inp = sass_list_get_value(s_args, 0);
  struct SassMapIterator* it = sass_map_make_iterator(inp);
  while (!sass_map_iterator_exhausted(it)) {
    SassValue* pair = sass_make_list(SASS_SPACE, false);
    sass_list_push(pair, sass_map_iterator_get_key(it));
    sass_list_push(pair, sass_map_iterator_get_value(it));
    sass_list_push(result, pair);
    sass_map_iterator_next(it);
  }
  sass_map_delete_iterator(it);
  return result;
}

// Function invoked on `@warn` hook (we are free to do with it whatever we want)
struct SassValue* fn_warn(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  sass_env_set_lexical(comp, "warn", s_args);
  return sass_value_stringify(s_args, false, 5);
}

struct SassValue* fn_debug(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  sass_env_set_lexical(comp, "debug", s_args);
  return sass_value_stringify(s_args, false, 5);
}

struct SassValue* fn_error(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  sass_env_set_lexical(comp, "error", s_args);
  return sass_value_stringify(s_args, false, 5);
}

// specific implementation of sass value ops
struct SassValue* fn_value_op(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  if (!sass_value_is_list(s_args)) {
    return sass_make_error("Invalid arguments for sass_op");
  }
  if (sass_list_get_size(s_args) != 3) {
    return sass_make_error("Exactly three arguments expected for sass_op");
  }
  struct SassValue* cmd = sass_list_get_value(s_args, 0);
  struct SassValue* lhs = sass_list_get_value(s_args, 1);
  struct SassValue* rhs = sass_list_get_value(s_args, 2);

  if (!sass_value_is_string(cmd)) {
    return sass_make_error("You must pass a string as operator");
  }
  std::string str(sass_string_get_value(cmd));
  enum SassOperator op = ADD;
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
struct SassValue* fn_value_stringify(struct SassValue* s_args, struct SassCompiler* comp, void*)
{
  if (!sass_value_is_list(s_args)) {
    return sass_make_error("Invalid arguments for stringify");
  }
  if (sass_list_get_size(s_args) != 1) {
    return sass_make_error("Exactly three arguments expected for stringify");
  }
  struct SassValue* ex = sass_list_get_value(s_args, 0);
  return sass_value_stringify(ex, false, 5);
}

// return version of libsass we are linked against
extern "C" const char* ADDCALL libsass_get_version() {
  return libsass_version();
}

// entry point for libsass to request custom functions from plugin
extern "C" void ADDCALL libsass_init_plugin(struct SassCompiler* compiler)
{
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_null($value)", fn_value_is_null, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_number($value)", fn_value_is_number, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_string($value)", fn_value_is_string, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_boolean($value)", fn_value_is_boolean, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_color($value)", fn_value_is_color, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_list($value)", fn_value_is_list, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_map($value)", fn_value_is_map, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_error($value)", fn_value_is_error, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_is_warning($value)", fn_value_is_warning, 0));

    sass_compiler_add_custom_function(compiler, sass_make_function("sass_number_get_value($number)", fn_number_get_value, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_number_set_value($number, $value)", fn_number_set_value, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_number_get_unit($number)", fn_number_get_unit, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_number_set_unit($number, $unit)", fn_number_set_unit, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_number_normalize($number)", fn_sass_number_normalize, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_number_reduce($number)", fn_sass_number_reduce, 0));

    sass_compiler_add_custom_function(compiler, sass_make_function("sass_string_get_value($string)", fn_string_get_value, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_string_set_value($string, $value)", fn_string_set_value, 0));

    sass_compiler_add_custom_function(compiler, sass_make_function("sass_boolean_get_value($bool)", fn_boolean_get_value, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_boolean_set_value($bool, $value)", fn_boolean_set_value, 0));

    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_get_r($color)", fn_color_get_r, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_get_g($color)", fn_color_get_g, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_get_b($color)", fn_color_get_b, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_get_a($color)", fn_color_get_a, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_set_r($color, $nr)", fn_color_set_r, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_set_g($color, $nr)", fn_color_set_g, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_set_b($color, $nr)", fn_color_set_b, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_color_set_a($color, $nr)", fn_color_set_a, 0));


    sass_compiler_add_custom_function(compiler, sass_make_function("sass_string_is_quoted($string)", fn_string_is_quoted, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_list_get_size($list)", fn_list_get_size, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_list_get_separator($list)", fn_list_get_separator, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_list_get_value($list, $i)", fn_list_get_value, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_list_set_value($list, $i, $value)", fn_list_set_value, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_map_iterate($map)", fn_sass_map_make_iterator, 0));

    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_stringify($value)", fn_value_stringify, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("sass_value_op($op, $lhs, $rhs)", fn_value_op, 0));

    sass_compiler_add_custom_function(compiler, sass_make_function("@warn($warn)", fn_warn, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("@debug($debug)", fn_debug, 0));
    sass_compiler_add_custom_function(compiler, sass_make_function("@error($error)", fn_error, 0));

}
