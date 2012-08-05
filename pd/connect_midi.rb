#!/usr/bin/env ruby

teensy = "24:0"
pd = "128:0"

system('aconnect -x')
unless system('aconnect', teensy, pd)
  puts "FAIL"
end
