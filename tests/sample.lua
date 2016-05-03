local cutil = require "cutil"

function filter_spec_chars(s)
	local ss = {}
	for k = 1, #s do
		local c = string.byte(s,k)
		if not c then break end
		if (c>=48 and c<=57) or (c>= 65 and c<=90) or (c>=97 and c<=122) then
			table.insert(ss, string.char(c))
		elseif c>=228 and c<=233 then
			local c1 = string.byte(s,k+1)
			local c2 = string.byte(s,k+2)
			if not c1 or not c2 then break end
			local a1,a2,a3,a4 = 128,191,128,191
			if c == 228 then a1 = 184 end
			if c == 233 then a2, a4 = 190, 165 end
			if c1>=a1 and c1<=a2 and c2>=a3 and c2<=a4 then
				k = k + 2
				table.insert(ss, string.char(c,c1,c2))
			end
		end
	end
	return table.concat(ss)
end


ss = '$s%sfoo"明天"a"你好、，。～！＠＃￥％……＆×（）｛｝｜：“《》？"foo%%23333|'

print("lua version: ", filter_spec_chars(ss))
print("cutil version: ", cutil.filter_spec_chars(ss))


print("efficiency comparison: ")
t = os.clock()
for k = 1,100000 do
	filter_spec_chars(ss)
end
t1 = os.clock()
print("test lua version: ", t1-t) -- 5.78

t = os.clock()
for k = 1,100000 do
	cutil.filter_spec_chars(ss)
end
t1 = os.clock()
print("test cutil version: ", t1-t) -- 0.06