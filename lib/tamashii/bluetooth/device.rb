# frozen_string_literal: true

module Tamashii
  module Bluetooth
    # Device
    class Device
      STATE = %i[
        unknown
        resetting
        unsupported
        unauthorized
        powered_off
        powered_on
      ].freeze

      def state
        STATE[@state]
      end
    end
  end
end
