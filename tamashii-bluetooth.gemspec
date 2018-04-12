
# frozen_string_literal: true

lib = File.expand_path('lib', __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'tamashii/bluetooth/version'

Gem::Specification.new do |spec|
  spec.name          = 'tamashii-bluetooth'
  spec.version       = Tamashii::Bluetooth::VERSION
  spec.authors       = ['蒼時弦也']
  spec.email         = ['elct9620@frost.tw']

  spec.summary       = 'The bluetooth implement for tamashii'
  spec.description   = 'The bluetooth implement for tamashii'
  spec.homepage      = 'https://tamashii.io'

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']
  spec.extensions    = ['ext/bluetooth/extconf.rb']

  spec.add_development_dependency 'bundler', '~> 1.16'
  spec.add_development_dependency 'overcommit', '~> 0.44.0'
  spec.add_development_dependency 'rake', '~> 10.0'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'rspec', '~> 3.0'
  spec.add_development_dependency 'rubocop', '~> 0.54.0'
end
