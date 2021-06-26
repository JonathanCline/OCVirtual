local computer = require("computer")
local component = require("component")

local gpu = component.proxy(component.list("gpu")())
local screen = component.list("screen")()
gpu.bind(screen, true)

local function size()
    return gpu.getViewport()
end
local function clear()
    local w, h = size()
    gpu.fill(1, 1, w, h, " ")
end


local terminal = { lines = {}, x = 1, y = 1 }
terminal.w, terminal.h = size()

function terminal.clear()
    local t = terminal
    gpu.fill(t.x, t.y, t.w, t.h, " ")
end
function terminal.flush()
    local t = terminal
    t.clear()
end


