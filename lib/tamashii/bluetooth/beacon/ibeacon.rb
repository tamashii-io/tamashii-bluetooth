# frozen_string_literal: true

module Tamashii
  module Bluetooth
    module Beacon
      # Apple iBeacon
      class IBeacon < Advertisment
        attr_reader :uuid, :major, :minior, :measured_power

        def initialize(uuid, major = 0, minior = 0, measured_power = -59)
          @uuid = uuid.delete('-')
          @major = major
          @minior = minior
          @measured_power = measured_power

          @data = create_data
        end

        def create_data
          [
            frame.length + 5,
            0xFF, 0x4C, 0x00, 0x02,
            frame.length,
            frame
          ].flatten
        end

        def frame
          @frame ||= [
            uuid_hex,
            int_to_hex(major, 2),
            int_to_hex(minior, 2),
            measured_power
          ].flatten
        end

        def int_to_hex(value, size)
          value
            .to_s(16)
            .rjust(size * 2, '0')
            .scan(/../)
            .map(&:hex)
        end

        def uuid_hex
          uuid.scan(/../).map(&:hex)
        end
      end
    end
  end
end
