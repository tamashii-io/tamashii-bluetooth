#include "../bluetooth.h"

xpc_object_t rb_create_xpc_data(VALUE object)
{
  int i;
  int length = (int)RARRAY_LEN(object);
  unsigned char *bytes;
  VALUE *ptr = RARRAY_PTR(object);

  bytes = malloc(sizeof(unsigned char) * length);

  for(i = 0; i < length; i++) {
    bytes[i] = (char)FIX2UINT(ptr[i]);
  }

  return xpc_data_create(bytes, length);
}

xpc_object_t rb_object_to_xpc_value(VALUE object)
{
  xpc_object_t xpc_object;

  switch(TYPE(object)) {
    case T_FIXNUM:
      xpc_object = xpc_int64_create(FIX2INT(object));
      break;
    case T_HASH:
      xpc_object = rb_hash_to_xpc_object(object);
      break;
    case T_STRING:
      xpc_object = xpc_string_create(StringValuePtr(object));
      break;
    case T_ARRAY:
      xpc_object = rb_create_xpc_data(object);
      break;
    default:
      rb_raise(rb_eTypeError, "Unsupported Type");
  }

  return xpc_object;
}

xpc_object_t rb_hash_to_xpc_object(VALUE hash)
{
  xpc_object_t xpc_object = xpc_dictionary_create(NULL, NULL, 0);
  xpc_object_t xpc_value;
  // TODO: Check rb_hash_keys location
  VALUE keys = rb_funcall(hash, rb_intern("keys"), 0);
  VALUE key, key_to_s, value, value_to_s;
  st_index_t size = RHASH_SIZE(hash);
  st_index_t i = 0;

  for(; i < size; i++) {
    key = rb_ary_entry(keys, i);
    key_to_s = rb_funcall(key, rb_intern("to_s"), 0);
    value = rb_hash_aref(hash, key);

    xpc_value = rb_object_to_xpc_value(value);
    xpc_dictionary_set_value(xpc_object, StringValuePtr(key_to_s), xpc_value);

    if(xpc_value) {
      xpc_release(xpc_value);
    }
  }

  return xpc_object;
}
