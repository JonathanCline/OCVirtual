local computer = require("computer")
local component = require("component")
local system = require("system")
local table = require("table")
local string = require("string")
local task, err = require("task")
local terminal = require("terminal")

if not task then error(err) end
assert(task.current() == 0, "kernel must be the root task!")

do  -- Setup the terminal
    local gpu = component.proxy(component.list("gpu")())
    local screen = component.list("screen")()
    if screen then gpu.bind(screen, true) end
    terminal.set_device(gpu)
    terminal.clear()
end
function print(...)
    local complete = table.concat(table.pack(...), " ")
    terminal.write(complete)
    terminal.refresh()
end

local function sleep(duration) computer.pullSignal(duration) end

local inet = component.proxy(component.list("internet")())
local address = "127.0.0.1"
local port = 6969

local conn = inet.connect(address, port)
conn.finishConnect()

local keepAlive = true
local signalFuncs = {}

function signalFuncs.shutdown()
    keepAlive = false
end
function signalFuncs.internet_ready(_, socketID)
    local msg = conn.read(512)
    print("got message!")
    if not msg then
        print("  ~ Lost Connection ~")
    else
        msg = string.gsub(msg, "[^:]+:", "", 1)
        print(msg)
    end
end

while keepAlive do
    local spack = table.pack(computer.pullSignal(1))
    if not spack[1] then
        local msg = conn.read(512)
        print(msg)
    end
    if spack[1] and signalFuncs[spack[1]] then
        local callback = signalFuncs[spack[1]]
        callback(table.unpack(spack))
    end
end

computer.shutdown()
