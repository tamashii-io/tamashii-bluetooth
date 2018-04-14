#ifndef TAMASHII_BLUETOOTH_INIT_H
#define TAMASHII_BLUETOOTH_INIT_H

#include "bluetooth.h"

VALUE rb_mTamashiiBluetooth;

VALUE rb_cTamashiiBluetoothDevice;
VALUE rb_cTamashiiBluetoothEvent;

VALUE rb_cTamashiiBluetoothAdvertisment;

// Initialize Extension
extern void Init_bluetooth(void)
{
  VALUE rb_mTamashii = rb_const_get(rb_cObject, rb_intern("Tamashii"));
  rb_mTamashiiBluetooth = rb_define_module_under(rb_mTamashii, "Bluetooth");

  rb_cTamashiiBluetoothEvent = rb_define_class_under(rb_mTamashiiBluetooth, "Event", rb_cObject);
  rb_define_attr(rb_cTamashiiBluetoothEvent, "device", 1, 0);
  rb_define_attr(rb_cTamashiiBluetoothEvent, "type", 1, 0);

  rb_cTamashiiBluetoothDevice = rb_define_class_under(rb_mTamashiiBluetooth, "Device", rb_cObject);
  rb_define_attr(rb_cTamashiiBluetoothDevice, "events", 1, 0);
  rb_define_alloc_func(rb_cTamashiiBluetoothDevice, rb_tamashii_bt_device_allocate);
  rb_define_method(rb_cTamashiiBluetoothDevice, "initialize", rb_tamashii_bt_device_initialize, 0);
  rb_define_method(rb_cTamashiiBluetoothDevice, "open", rb_tamashii_bt_device_open, 0);
  rb_define_method(rb_cTamashiiBluetoothDevice, "advertising", rb_tamashii_bt_device_advertising, 1);
  rb_define_method(rb_cTamashiiBluetoothDevice, "stop", rb_tamashii_bt_device_stop, 0);

  rb_cTamashiiBluetoothAdvertisment = rb_define_class_under(rb_mTamashiiBluetooth, "Advertisment", rb_cObject);
  rb_define_attr(rb_cTamashiiBluetoothAdvertisment, "data", 1, 1);
}

#endif
