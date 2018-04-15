# frozen_string_literal: true

module Tamashii
  module Bluetooth
    # Beacon Support
    module Beacon
      autoload :LINE, 'tamashii/bluetooth/beacon/line'
      autoload :IBeacon, 'tamashii/bluetooth/beacon/ibeacon'
    end
  end
end
