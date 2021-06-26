local debug = debug
local main
do
	local computer = computer
	local cinv = component.invoke
	local huge = math.huge
	local match = string.match

	local bootfs = computer.getBootAddress()
	local function raw_loadfile(path)
		local file = cinv(bootfs, "open", path)
		local fdata = ""
		while true do
			local fchunk = cinv(bootfs, "read", file, huge)
			if fchunk then fdata = fdata .. fchunk else goto exit end
		end
		::exit::
		cinv(bootfs, "close", file)
		return load(fdata, "=" .. match(path, "/(%w+).lua"))
	end
	main = raw_loadfile("system/boot.lua")(raw_loadfile)
end
local good, msg = xpcall(main, debug.traceback)
if not good then error(msg) end
