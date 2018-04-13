#ifndef TAMASHII_BLUETOOTH_OSX_H
#define TAMASHII_BLUETOOTH_OSX_H 1

#include "ruby.h"
#include "xpc/xpc.h"

#define BLUED_STATE_CHANGED 6
#define BLUED_MSG_INIT 1

#ifdef OSX_YOSEMITE
#define BLUED_MSG_ADVERTISING 8
#else
#define BLUED_MSG_ADVERTISING 16
#endif

static const char* service_name = "com.apple.blued";

typedef struct tamashii_bluetooth_device {
  xpc_connection_t connection;
} tamatshii_bt_device_t;

typedef struct tamashii_bluetooth_event {
  xpc_object_t object;
  xpc_type_t type;
} tamashii_bt_event_t;

// TODO: Refactor below methods
xpc_object_t rb_hash_to_xpc_object(VALUE);
xpc_object_t rb_object_to_xpc_value(VALUE);
xpc_object_t rb_create_xpc_data(VALUE);

xpc_object_t tamashii_create_message_from_advertisment(VALUE);

void rb_tamashii_bt_event_free(void *);
VALUE rb_tamashii_bt_event_create(xpc_object_t, VALUE);

VALUE resolve_tamashii_bt_event(VALUE self, tamashii_bt_event_t*);
#endif
