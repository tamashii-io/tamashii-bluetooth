# frozen_string_literal: true

module Tamashii
  module Bluetooth
    class Device
      STATE = %i[
        unknown
        resetting
        unsupported
        unauthorized
        powered_off
        powered_on
      ].freeze
    end
  end
end
