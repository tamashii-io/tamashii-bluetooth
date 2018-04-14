# frozen_string_literal: true

module Tamashii
  module Bluetooth
    module Beacon
      # Line Beacon Format
      class LINE < Advertisment
        ADTYPE_FLAGS = 0x01
        ADTYPE_COMPLETE_16_BIT_SERVICE_UUID = 0x03
        ADTYPE_SERVICE_DATA = 0x16
        UUID16LE_FOR_LINECORP = [0x6F, 0xFE].freeze

        attr_reader :hardware_id

        def initialize(id)
          # TODO: Support append message
          @hardware_id = id
          @data = create_data
        end

        def frame_type
          [0x02]
        end

        def tx_power
          [0x7F]
        end

        def hardware_hex
          hardware_id.scan(/../).map(&:hex)
        end

        def beacon_frame
          [
            frame_type,
            hardware_hex,
            tx_power,
            [0x00] # Message
          ].flatten
        end

        def create_data
          [
            flag,
            completed_uuid,
            service_data
          ].flatten
        end

        def flag
          add_structure(ADTYPE_FLAGS, [0x06])
        end

        def completed_uuid
          add_structure(
            ADTYPE_COMPLETE_16_BIT_SERVICE_UUID,
            UUID16LE_FOR_LINECORP
          )
        end

        def service_data
          add_structure(
            ADTYPE_SERVICE_DATA,
            UUID16LE_FOR_LINECORP + beacon_frame
          )
        end

        def add_structure(type, buffer)
          header = [buffer.length + 1, type]
          [header, buffer]
        end
      end
    end
  end
end
