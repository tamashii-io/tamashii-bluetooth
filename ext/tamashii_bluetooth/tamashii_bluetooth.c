#include "tamashii_bluetooth.h"

VALUE rb_mTamashiiBluetooth;

void
Init_tamashii_bluetooth(void)
{
  rb_mTamashiiBluetooth = rb_define_module("TamashiiBluetooth");
}
