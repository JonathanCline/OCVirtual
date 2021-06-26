local raw_loadfile = ...

do
    local bootfs = computer.getBootAddress()
    local cinv = component.invoke
    local exists = function(_path)
        return cinv(bootfs, "exists", _path)
    end
    local pacman = raw_loadfile("/system/lib/package.lua")(raw_loadfile, exists)
    assert(type(pacman) == "table", "failed to load package manager file")
    require = pacman.require
end

return raw_loadfile("/system/kernel.lua")