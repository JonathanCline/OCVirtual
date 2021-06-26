--- Expects load file and exists function to be passed in
local loadfile, exists = ...

assert(type(loadfile) == "function", "bad loadfile function")
assert(type(exists) == "function", "bad exists function")

local package = {}

-- Mark global libraries as loaded
package.loaded = {}
do
    local libs =
    {
        "computer", "component", "table", "string", "coroutine", "bit32", "unicode", "os", "debug", "math"
    }
    for i, v in ipairs(libs) do
        assert(type(_G[v]) == "table", v .. " is not a table!")
        package.loaded[v] = _G[v]
        _G[v] = nil
    end
end
package.loaded.package = package

-- Search paths for finding libraries
package.path =
{
    "system/lib/"
}

---@param _name string Name to find, searches down package.path list
local function resolve(_name)
    for i, v in ipairs(package.path) do
        local path = v .. _name .. ".lua"
        if exists(path) then
            return path
        end
    end
    error("no path found for " .. _name)
end

---Loads a library table
---@param _name string Name of the library
---@param _from? string Path to the library script, if not provided this is set to resolve(_name)
---@return table
local function load(_name, _from)
    _from = _from or resolve(_name)
    if not _from then
        return nil, "bad path"
    end
    local _lib, _err = loadfile(_from, "=" .. _name)
    if not _lib then
        return nil, _err
    else
        _lib = _lib()
        if type(_lib) ~= "table" then
            return nil, "library did not return a table"
        end
    end
    return _lib
end

---@param _name string Name of the library to get
function package.require(_name)
    if package.loaded[_name] then -- Return if already loaded
        return package.loaded[_name]
    else -- Load in library
        local _lib, _err = load(_name)
        if _lib then
            package.loaded[_name] = _lib
            return _lib
        else
            return nil, _err
        end
    end
end

return package