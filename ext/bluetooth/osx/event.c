#include "../bluetooth.h"

VALUE resolve_tamashii_bt_event(VALUE self, tamashii_bt_event_t *event) {
  int msg_id = xpc_dictionary_get_int64(event->object, "kCBMsgId");
  char* type;

  // TODO: Refactor Event Type define
  switch(msg_id) {
    case BLUED_STATE_CHANGED:
      {
        xpc_object_t args = xpc_dictionary_get_value(event->object, "kCBMsgArgs");

        int state = xpc_dictionary_get_int64(args, "kCBMsgArgState");
        VALUE device = rb_ivar_get(self, rb_intern("@device"));
        rb_ivar_set(device, rb_intern("@state"), INT2FIX(state));

        type = "state_changed";
        break;
      }
    default:
      // TODO: Add all supported event
      type = "unknown";
      return Qnil;
  }

  rb_ivar_set(self, rb_intern("@type"), ID2SYM(rb_intern(type)));
  return Qtrue;
}

void rb_tamashii_bt_event_free(void *ptr)
{
  tamashii_bt_event_t *event = (tamashii_bt_event_t*)ptr;
  xpc_release(event->object);
  free(event);
}

VALUE rb_tamashii_bt_event_create(xpc_object_t xpc_event, VALUE device)
{
  VALUE rb_event;
  // TODO: Is it necessary allocate memory or simple reference xpc object?
  tamashii_bt_event_t *event = malloc(sizeof(tamashii_bt_event_t));
  event->object = xpc_event;
  event->type = xpc_get_type(xpc_event);

  rb_event = Data_Wrap_Struct(rb_cTamashiiBluetoothEvent, 0, rb_tamashii_bt_event_free, event);
  rb_ivar_set(rb_event, rb_intern("@device"), device);

  if(event->type == XPC_TYPE_ERROR) {
    const char* message = "Unknow Error";

    if (event->object == XPC_ERROR_CONNECTION_INTERRUPTED) {
      message = "Connection Interrupted";
    } else if (event->object == XPC_ERROR_CONNECTION_INVALID) {
      message = "Connection Invalid";
    }

    rb_raise(rb_eRuntimeError, "%s", message);
  } else if (event->type == XPC_TYPE_DICTIONARY) {
    resolve_tamashii_bt_event(rb_event, event);
  }

  return rb_event;
}


