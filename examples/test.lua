local lbuf = require"buf"

local buf = lbuf.new("Hello world!")

print("length:", buf:length())
print("size:", buf:size())
print("data = '" .. tostring(buf) .. "'")
print("data:sub(2,4) = '" .. buf:sub(2,4) .. "'")
print("reset:", buf:reset())
print("length:", buf:length())
print("size:", buf:size())
print("data = '" .. tostring(buf) .. "'")

local format = string.format
local sbyte = string.byte
local rep = string.rep
function tohex(buf)
	buf = tostring(buf)
	local out = ''
	for byte=1, #buf, 16 do
		local chunk = buf:sub(byte, byte+15)
		out = out .. format('%08X  ',byte-1)
		out = out .. chunk:gsub('.', function (c) return format('%02X ',sbyte(c)) end)
		out = out .. rep(' ',3*(16-#chunk))
		out = out .. ' ' .. chunk:gsub('[^%w%p ]','.') .. "\n" 
	end
	return out
end

local function append_vals(buf, ctype, vals, zig)
	local append = assert(buf['append_' .. ctype])
	for i=1,#vals do
		local val = vals[i]
		print("append " .. (zig and 'zigzag ' or '') .. ctype ..":", val, append(buf, val, zig))
	end
end

local function read_vals(buf, ctype, vals, zig)
	local read = assert(buf['read_' .. ctype])
	for i=1,#vals do
		local val = vals[i]
		if ctype == 'data' then
			print("read data:", val, read(buf, #val))
		else
			print("read " .. (zig and 'zigzag ' or '') .. ctype ..":", val, read(buf, zig))
		end
	end
end

local function run_test(test)
	local ctype = test[1]
	local vals = test[2]
	local zig = test[3]
	append_vals(buf, ctype, vals, zig)
	print("data = [\n" .. tohex(buf) .. "]")
	read_vals(buf, ctype, vals, zig)
	print("length:", buf:length())
end

-- Test append/read
buf:reset()
local tests = {
	{ 'uint8',       { 65, 66, 67, -1 }, },
	{ 'uint16',      { 123, 1234, 123456789, -1 }, },
	{ 'uint32',      { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'uint64',      { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'int8',        { 65, 66, 67, -1 }, },
	{ 'int16',       { 123, 1234, 123456789, -1 }, },
	{ 'int32',       { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'int64',       { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'float',       { 12.3, 12.34, 12.3456789, 12.34567890123, -1 }, },
	{ 'double',      { 12.3, 12.34, 12.3456789, 12.34567890123, -1 }, },
	{ 'b128_uvar32', { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'b128_uvar64', { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'b128_var32',  { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'b128_var64',  { 123, 1234, 123456789, 1234567890123, -1 }, },
	{ 'b128_var32',  { 123, 1234, 123456789, 1234567890123, -1 }, true, },
	{ 'b128_var64',  { 123, 1234, 123456789, 1234567890123, -1 }, true, },
	{ 'data',        { "123", " Hello", ", world", "!" }, },
	{ 'string',      { "123", " Hello", ", world", "!" }, },
}
for n=1,#tests do run_test(tests[n]) end

-- test growing the buffer.
local size = buf:size()
local target_size = size * 5

repeat
	buf:append_data("xXxXxXxXxXxXxX")
	size = buf:size()
until size >= target_size
print("size:", buf:size())
print("length:", buf:length())

