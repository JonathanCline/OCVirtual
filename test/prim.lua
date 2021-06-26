
local component = component
local string = string

do
    local raw_list = component.list
    function component.list(pattern)
        local raw = raw_list()
        return function()
            local n, t = raw()
            while n ~= nil and not string.match(t, pattern) do
                n, t = raw()
            end
            return n, t
        end
    end
    function component.proxy(address)
        local o =
        {
            address = address,
            type = component.type(address)
        }
        local mt =
        {
            __index = function(self, key)
                return function(...)
                    return component.invoke(o.address, key, ...)
                end
            end
        }
        return setmetatable(o, mt)
    end
end

local computer = component.proxy(component.list("computer")())

print(computer.type)

print("methods")
for i, v in ipairs(component.methods(computer.address)) do
    print("\t" .. v)
end

