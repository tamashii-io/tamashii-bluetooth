#ifndef TAMASHII_BLUETOOTH_H
#define TAMASHII_BLUETOOTH_H

#include <stdlib.h>

#include "ruby.h"
#include "ruby/intern.h"
#include "ruby/thread.h"

#ifdef OS_OSX
#include "osx.h"
#endif

#include "event.h"

// TODO: Prevent crash if program stop after initialize device

// Ruby Object Define
extern VALUE rb_mTamashiiBluetooth;

extern VALUE rb_cTamashiiBluetoothDevice;
extern VALUE rb_cTamashiiBluetoothEvent;

// Device Methods
VALUE rb_tamashii_bt_device_allocate(VALUE);
void rb_tamashii_bt_device_deallocate(void*);

VALUE rb_tamashii_bt_device_initialize(VALUE);
VALUE rb_tamashii_bt_device_send(VALUE, VALUE);

// Event Methods

#endif /* TAMASHII_BLUETOOTH_H */
