def grep_with_line(file_name,keys)
	result = Array.new
	File.readlines(file_name).each_with_index do |line,index|
		keys.each do |key|
			if line.include?(key) then
				result.push(index.to_s+": "+line)
			end
		end
	end
	return result
end

# main loop
if ARGV.length != 2 then
  puts "Usage $0 <input directory> <output file name>"
  exit 1
end

file_path = ARGV[0]
output_file = ARGV[1]
num = 0

File.open(output_file,"w") do |fp|
	Dir.glob(file_path+"/*") do |f|
		result = grep_with_line(f,["todo","doNext","debug"])
		if !result.empty? then
			fp.puts File.basename(f)
			result.each do |v|
				fp.puts v
			end
			fp.puts ""
			num += result.length
		end
	end
end

puts "未実装項目数: "+num.to_s