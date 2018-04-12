#include "../bluetooth.h"

xpc_object_t rb_create_xpc_data(VALUE object)
{
  int i;
  int length = (int)RARRAY_LEN(object);
  unsigned char *bytes;
  VALUE *ptr = RARRAY_PTR(object);

  bytes = malloc(sizeof(unsigned char) * length);

  printf("Buffer: ");
  for(i = 0; i < length; i++) {
    bytes[i] = (char)FIX2UINT(ptr[i]);
    printf("%02x ", bytes[i]);
  }
  printf("\n");

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

static void deallocate(void *bt) {
  // TODO: Should xpc_connection release?
  free(bt);
}

static VALUE rb_tamashii_bt_device_allocate(VALUE klass)
{
  tamatshii_bt_device_t * bt = malloc(sizeof(tamatshii_bt_device_t));

  return Data_Wrap_Struct(klass, NULL, deallocate, bt);
}

static VALUE rb_tamashii_bt_device_send(VALUE self, VALUE hash)
{
  tamatshii_bt_device_t *bt;
  Data_Get_Struct(self, tamatshii_bt_device_t, bt);
  xpc_object_t message = rb_hash_to_xpc_object(hash);
  xpc_connection_send_message(bt->connection, message);

  return Qtrue;
}

void rb_free_xpc_event(void *ptr)
{
  struct rb_xpc_event_t *rb_xpc_event = (struct rb_xpc_event_t*)ptr;
  xpc_release(rb_xpc_event->event);
  free(rb_xpc_event);
}

VALUE rb_create_xpc_event(xpc_object_t event)
{
  struct rb_xpc_event_t *rb_xpc_event = malloc(sizeof(struct rb_xpc_event_t));
  rb_xpc_event->event = event;
  return Data_Wrap_Struct(rb_cTamashiiBluetoothEvent, 0, rb_free_xpc_event, rb_xpc_event);
}


static VALUE rb_tamashii_bt_device_initialize(VALUE self)
{
  tamatshii_bt_device_t * bt;
  Data_Get_Struct(self, tamatshii_bt_device_t, bt);

  bt->connection = xpc_connection_create_mach_service(service_name, NULL, XPC_CONNECTION_MACH_SERVICE_PRIVILEGED);

  VALUE rb_cQueue = rb_const_get(rb_cObject, rb_intern("Queue"));
  VALUE queue = rb_class_new_instance(0, NULL, rb_cQueue);
  rb_ivar_set(self, rb_intern("@events"), queue);
  rb_ivar_set(self, rb_intern("@state"), INT2FIX(0));

  xpc_connection_set_event_handler(bt->connection, ^(xpc_object_t event) {
      xpc_retain(event);
      VALUE tamashii_event = rb_create_xpc_event(event);
      rb_ivar_set(tamashii_event, rb_intern("@device"), self);
      rb_funcall(queue, rb_intern("push"), 1, tamashii_event);
  });

  xpc_connection_resume(bt->connection);

  return self;
}

VALUE rb_xpc_object_to_value(xpc_object_t object)
{
  VALUE value;

  xpc_type_t value_type = xpc_get_type(object);

  if (value_type == XPC_TYPE_INT64) {
    value = INT2FIX(xpc_int64_get_value(object));
  } else if (value_type == XPC_TYPE_STRING) {
    value = rb_str_new2(xpc_string_get_string_ptr(object));
  } else if (value_type == XPC_TYPE_DICTIONARY) {
    value = rb_xpc_dictionary_to_hash(object);
  } else {
    value = Qnil;
  }

  return value;
}

VALUE rb_xpc_dictionary_to_hash(xpc_object_t dictionary) {
  VALUE hash = rb_hash_new();

  xpc_dictionary_apply(dictionary, ^bool(const char *key, xpc_object_t value) {
      rb_hash_aset(hash, ID2SYM(rb_intern(key)), rb_xpc_object_to_value(value));
      return true;
  });

  return hash;
}

static VALUE rb_tamashii_bt_event_process(VALUE self)
{
  struct rb_xpc_event_t *rb_xpc_event;
  Data_Get_Struct(self, struct rb_xpc_event_t, rb_xpc_event);

  xpc_type_t event_type = xpc_get_type(rb_xpc_event->event);

  if(event_type == XPC_TYPE_ERROR) {
    const char* message = "Unknow Error";

    if (rb_xpc_event->event == XPC_ERROR_CONNECTION_INTERRUPTED) {
      message = "Connection Interrupted";
    } else if ( rb_xpc_event->event == XPC_ERROR_CONNECTION_INVALID) {
      message = "Connection Invalid";
    }

    rb_raise(rb_eException, "%s", message);
  } else if (event_type == XPC_TYPE_DICTIONARY) {

    // Update State
    if (xpc_dictionary_get_int64(rb_xpc_event->event, "kCBMsgId") == 6) {
      xpc_object_t args = xpc_dictionary_get_value(rb_xpc_event->event, "kCBMsgArgs");
      int state = xpc_dictionary_get_int64(args, "kCBMsgArgState");
      VALUE device = rb_ivar_get(self, rb_intern("@device"));
      rb_ivar_set(device, rb_intern("@state"), INT2FIX(state));
    }

    return rb_xpc_dictionary_to_hash(rb_xpc_event->event);
  }
  return Qfalse;
}
