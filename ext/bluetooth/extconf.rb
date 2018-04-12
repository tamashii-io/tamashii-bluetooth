# frozen_string_literal: true

require 'mkmf'

def add_define(name)
  $defs.push("-D#{name.upcase}")
end

os = case RUBY_PLATFORM
     when /linux/ then
       add_define 'os_linux'
       abort 'Linux support is comming soon'
     when /darwin/ then
       add_define 'os_osx'
       'osx'
     else
       abort "Unsupport platform #{RUBY_PLATFORM}"
     end

source_path = Pathname.new(__FILE__).expand_path.dirname.join(os)
create_makefile('tamashii/bluetooth/bluetooth', source_path.to_s)
