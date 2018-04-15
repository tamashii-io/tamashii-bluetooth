# Tamashii Bluetooth

The Bluetooth implement for Tamashii to building IoT device with BLE (Bluetooth Low Engery) function.

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'tamashii-bluetooth'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install tamashii-bluetooth

## Usage

Current only support running on macOS by using iBeacon format.

### Initialize Bluetooth device

```ruby
require 'tamashii/bluetooth'

device = Tamashii::Bluetooth::Device.new
device.open

# Do something ...

# This is async in macOS
device.stop
```

### Handling Event

```ruby
require 'tamashii/bluetooth'

device = Tamashii::Bluetooth::Device.new
device.open

# Ensure your ruby is running otherwise the Bluetooth device will be closed
Thread.new do
  until device.events.empty?
    event = device.events.pop

    if event.type == :state_changed && device.powered_on?
      # Advertising
    end
  end

  # Free CPU
  sleep 1
end.join
```

### Advertising (LINE Beacon)

```ruby
require 'tamashii/bluetooth'
require 'tamashii/bluetooth/beacon'

device = Tamashii::Bluetooth::Device.new
device.open

# beacon = Tamashii::Bluetooth::Advertisment.new
# beacon.data = [0x02, ...]
beacon = Tamashii::Bluetooth::Beacon::LINE.new('001facddd')

# Ensure your ruby is running otherwise the Bluetooth device will be closed
Thread.new do
  until device.events.empty?
    event = device.events.pop

    if event.type == :state_changed && device.powered_on?
      device.advertising(beacon)
    end
  end

  # Free CPU
  sleep 1
end.join
```

## Roadmap

### MVP (~> 0.1)

* [x] macOS support (XPC / CoreBluetooth)
* [ ] Linux support (libbluetooth)
* [ ] Advertising
  * [x] iBeacon
    * [x] iBeacon Class
    * [x] LINE Beacon Class
  * [ ] EddyStone
* [ ] Tests
* [ ] Events
  * [x] State Changes
  * [x] Start
  * [x] Stop
* [ ] Error Handling

### Full ( >= 1.0 )

We plan to add full Bluetooth support with breaking changes with MVP version.

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/tamashii-io/tamashii-bluetooth.
