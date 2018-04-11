#include "bluetooth.h"

VALUE rb_mTamashii;
VALUE rb_mBluetooth;

VALUE rb_cXPC;

void
Init_tamashii_bluetooth(void)
{
  rb_mTamashii = rb_define_module("Tamashii");
  rb_mBluetooth = rb_define_module_under(rb_mTamashii, "Bluetooth");

  rb_cXPC = rb_define_class_under(rb_mBluetooth, "XPC", rb_cObject);
}
