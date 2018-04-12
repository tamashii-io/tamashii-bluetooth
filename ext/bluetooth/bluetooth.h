#ifndef TAMASHII_BLUETOOTH_H
#define TAMASHII_BLUETOOTH_H 1

#include <stdlib.h>

#include "ruby.h"
#include "ruby/intern.h"
#include "ruby/thread.h"

#ifdef OS_OSX
#include "osx.h"
#endif

// Ruby Object Define
VALUE rb_mTamashiiBluetooth;

VALUE rb_cTamashiiBluetoothDevice;
VALUE rb_cTamashiiBluetoothEvent;

// Device Methods
static VALUE rb_tamashii_bt_device_allocate(VALUE);
static VALUE rb_tamashii_bt_device_initialize(VALUE);
static VALUE rb_tamashii_bt_device_send(VALUE, VALUE);

// Event Methods
static VALUE rb_tamashii_bt_event_process(VALUE);

// Initialize Extension
void
Init_bluetooth(void)
{
  VALUE rb_mTamashii = rb_const_get(rb_cObject, rb_intern("Tamashii"));
  rb_mTamashiiBluetooth = rb_define_module_under(rb_mTamashii, "Bluetooth");

  rb_cTamashiiBluetoothEvent = rb_define_class_under(rb_mTamashiiBluetooth, "Event", rb_cObject);
  // TODO: Remove unused method
  rb_define_method(rb_cTamashiiBluetoothEvent, "process", rb_tamashii_bt_event_process, 0);

  rb_cTamashiiBluetoothDevice = rb_define_class_under(rb_mTamashiiBluetooth, "Device", rb_cObject);
  rb_define_attr(rb_cTamashiiBluetoothDevice, "events", 1, 0);
  rb_define_alloc_func(rb_cTamashiiBluetoothDevice, rb_tamashii_bt_device_allocate);
  rb_define_method(rb_cTamashiiBluetoothDevice, "initialize", rb_tamashii_bt_device_initialize, 0);
  // TODO: Remove OSX XPC only method
  rb_define_method(rb_cTamashiiBluetoothDevice, "send", rb_tamashii_bt_device_send, 1);
}

#endif /* TAMASHII_BLUETOOTH_H */
