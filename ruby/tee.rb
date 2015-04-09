#!/usr/bin/env ruby

require 'optparse'

options = {}
OptionParser.new do |opts|
  opts.banner = "Usage: tee.rb [FILE]..."

  opts.on("-a", "--append", "Append output") do |v|
    options[:append] = true
  end
end.parse!

files = ARGV.map do |arg|
  if options[:append].nil?
    File.open(arg, "w")
  else
    File.open(arg, "a")
  end
end

while str = STDIN.read(4096)
  STDOUT.write(str)
  files.each do |f|
    f.write(str)
  end
end
