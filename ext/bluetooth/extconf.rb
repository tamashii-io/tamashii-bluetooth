# frozen_string_literal: true

require 'mkmf'
require 'pathname'

def add_define(name)
  $defs.push("-D#{name.upcase}")
end

def define_os_version
  version = RUBY_PLATFORM[/[[:digit:]]+$/].to_i
  add_define('osx_yosemite') if version < 17
end

os = case RUBY_PLATFORM
     when /linux/ then
       add_define 'os_linux'
       abort 'Linux support is comming soon'
     when /darwin/ then
       add_define 'os_osx'
       define_os_version
       'osx'
     else
       abort "Unsupport platform #{RUBY_PLATFORM}"
     end

source_path = Pathname.new(__FILE__).expand_path.dirname.join(os)
create_makefile('tamashii/bluetooth/bluetooth', source_path.to_s)
