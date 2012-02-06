#!/usr/bin/env lua
MAX_RANGE = 1000000

function test_dict()
	--[2012-02-04] set 2.11s get 0.87s clear 0.19s
	io.write("---- test_dict -----\n")
	local i = 0
	local d = {}
	local s
	local start
	start = os.clock()
	io.write(string.format("test dict set (%d)...\t", MAX_RANGE))
	for i=0, MAX_RANGE do
		s = string.format("%d", i)
		--print(s)
		d[s] = s
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	start = os.clock()
	io.write(string.format("test dict get (%d)...\t", MAX_RANGE))
	for i=0, MAX_RANGE do
		s = d[string.format("%d", i)]
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	start = os.clock()
	io.write(string.format("test dict clear (%d)...\t", MAX_RANGE))
	for i=0, MAX_RANGE do
		table.remove(d, i)
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	io.write("\n")
end

function test_list()
	--[2012-02-04] set 0.33s get 0.03s each 0.09s
	io.write("---- test_list -----\n")
	local i = 0
	local j
	local l = {}
	local s
	local start
	start = os.clock()
	io.write(string.format("test list set (%d)...\t", MAX_RANGE))
	for i=0, MAX_RANGE do
		table.insert(l, i)
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	start = os.clock()
	io.write(string.format("test list get (%d)...\t", MAX_RANGE))
	for i=0, MAX_RANGE do
		j = l[i]
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	start = os.clock()
	io.write(string.format("test list each (%d)...\t", MAX_RANGE))
	for i, j in pairs(l) do
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	io.write("\n")
end

function test_str()
	--[2012-02-05] find 0.94s replace 1.10s
	io.write("---- test_str -----\n")
	local i = 0
	local s = "1143534=69789+35435^(100&10+5)"
	local rep_before = "35"
	local rep_after = "3535"
	start = os.clock()
	io.write(string.format("test str find (%d)...\t", MAX_RANGE))
	for i=0, MAX_RANGE do
		string.find(s, string.format("%d", i))
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	--s = string.gsub(s, rep_before, rep_after)
	--print(s)
	start = os.clock()
	io.write(string.format("test str replace (%d)...\t", MAX_RANGE))
	for i=MAX_RANGE/2, MAX_RANGE do
		s = string.gsub(s, rep_before, rep_after)
		s = string.gsub(s, rep_after, rep_before)
	end
	io.write(string.format("%.2fs\n", os.clock()-start))
	io.write("\n")
end

if not package.loaded[...] then
	test_dict()
	test_list()
	test_str()
end