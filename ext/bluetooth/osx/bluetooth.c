#include "../bluetooth.h"
#include "../init.h"

xpc_object_t tamashii_create_message(int message_id, xpc_object_t args)
{
  xpc_object_t message = xpc_dictionary_create(NULL, NULL, 0);

  xpc_dictionary_set_value(message, "kCBMsgId", xpc_int64_create(message_id));
  xpc_dictionary_set_value(message, "kCBMsgArgs", args);

  return message;
}

xpc_object_t tamashii_create_message_from_advertisment(VALUE object)
{
  xpc_object_t args = xpc_dictionary_create(NULL, NULL, 0);
  VALUE data = rb_ivar_get(object, rb_intern("@data"));

  xpc_dictionary_set_value(args, "kCBAdvDataAppleMfgData", rb_create_xpc_data(data));

  return tamashii_create_message(BLUED_MSG_ADVERTISING, args);
}

void rb_tamashii_bt_device_deallocate(void *bt)
{
  free(bt);
}

VALUE rb_tamashii_bt_device_allocate(VALUE klass)
{
  tamatshii_bt_device_t * bt = malloc(sizeof(tamatshii_bt_device_t));

  return Data_Wrap_Struct(klass, NULL, rb_tamashii_bt_device_deallocate, bt);
}

VALUE rb_tamashii_bt_device_open(VALUE self)
{
  tamatshii_bt_device_t *bt;
  Data_Get_Struct(self, tamatshii_bt_device_t, bt);

  xpc_object_t args = xpc_dictionary_create(NULL, NULL, 0);
  xpc_object_t options = xpc_dictionary_create(NULL, NULL, 0);

  char* name;
  name = malloc(13 + sizeof(time_t) * 2);
  sprintf(name, "tamashii-ble-%ld", time(NULL));

  xpc_dictionary_set_value(options, "kCBInitOptionShowPowerAlert", xpc_int64_create(1));

  xpc_dictionary_set_value(args, "kCBMsgArgName", xpc_string_create(name));
  xpc_dictionary_set_value(args, "kCBMsgArgType", xpc_int64_create(1));
  xpc_dictionary_set_value(args, "kCBMsgArgOptions", options);

  xpc_connection_send_message(bt->connection, tamashii_create_message(BLUED_MSG_INIT, args));
  return Qnil;
}

VALUE rb_tamashii_bt_device_advertising(VALUE self, VALUE object)
{
  tamatshii_bt_device_t *bt;
  Data_Get_Struct(self, tamatshii_bt_device_t, bt);

  xpc_object_t message;

  if (rb_obj_is_kind_of(object, rb_cTamashiiBluetoothAdvertisment)) {
    message = tamashii_create_message_from_advertisment(object);
  }

  xpc_connection_send_message(bt->connection, message);
  return Qnil;
}

VALUE rb_tamashii_bt_device_initialize(VALUE self)
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
      VALUE tamashii_event = rb_tamashii_bt_event_create(event, self);
      rb_funcall(queue, rb_intern("push"), 1, tamashii_event);
  });

  xpc_connection_resume(bt->connection);

  return self;
}
