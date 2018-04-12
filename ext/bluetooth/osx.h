#ifndef TAMASHII_BLUETOOTH_MACOSX_H
#define TAMASHII_BLUETOOTH_MACOSX_H 1

#include "ruby.h"
#include "xpc/xpc.h"

const char* service_name = "com.apple.blued";

typedef struct tamashii_bluetooth_device {
  xpc_connection_t connection;
} tamatshii_bt_device_t;

// TODO: Refactor below methods
xpc_object_t rb_hash_to_xpc_object(VALUE);
xpc_object_t rb_object_to_xpc_value(VALUE);

struct rb_xpc_event_t {
  xpc_object_t event;
};

void rc_free_xpc_event(void *);
VALUE rb_xpc_create_event(xpc_object_t);

VALUE rb_xpc_object_to_value(xpc_object_t);
VALUE rb_xpc_dictionary_to_hash(xpc_object_t);

#endif
