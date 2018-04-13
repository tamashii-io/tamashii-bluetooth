#include "../bluetooth.h"
#include "../init.h"

void rb_tamashii_bt_device_deallocate(void *bt) {
  free(bt);
}

VALUE rb_tamashii_bt_device_allocate(VALUE klass)
{
  tamatshii_bt_device_t * bt = malloc(sizeof(tamatshii_bt_device_t));

  return Data_Wrap_Struct(klass, NULL, rb_tamashii_bt_device_deallocate, bt);
}

VALUE rb_tamashii_bt_device_send(VALUE self, VALUE hash)
{
  tamatshii_bt_device_t *bt;
  Data_Get_Struct(self, tamatshii_bt_device_t, bt);
  xpc_object_t message = rb_hash_to_xpc_object(hash);
  xpc_connection_send_message(bt->connection, message);

  return Qtrue;
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
