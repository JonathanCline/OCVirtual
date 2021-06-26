local terminal =
{
    x = 1,
    y = 1,
    w = 0,
    h = 0,
    lines = {},
    cursor =
    {
        x = 1,
        y = 1
    }
}

local string = require("string")
local unicode = require("unicode")

do
    local terminalDevice = nil

    ---@return table :terminal's display device
    function terminal.device()
        return terminalDevice
    end
    ---@return table :previous terminal device
    function terminal.set_device(gpuProc)
        local old = terminalDevice
        terminalDevice = gpuProc
        if gpuProc then
            terminal.update_resolution()
        end
        return old
    end
end

--- Updates the terminal's bounds if the display device has been set
function terminal.update_resolution()
    local dv = terminal.device()
    if dv then
        terminal.w, terminal.h = dv.getResolution()
    end
    return terminal.w, terminal.h
end

--- Clears the terminal's output
function terminal.clear()
    local dv = terminal.device()
    if dv then
        dv.fill(terminal.x, terminal.y, terminal.w, terminal.h, " ")
    end
    terminal.cursor.x = 1
    terminal.cursor.y = 1
    return dv ~= nil
end

function terminal.write(message)
    local dv = terminal.device()
    if dv then
        local lines = terminal.lines
        for line in string.gmatch(tostring(message), "[^\n]*") do
            if unicode.len(line) > 0 then
                lines[#lines + 1] = line
            end
        end
    end
end

local function print(str)
    local dv = terminal.device()
    if dv then
        local cursor = terminal.cursor
        dv.set(cursor.x + terminal.x - 1, cursor.y + terminal.y - 1, str)
        cursor.x = cursor.x + unicode.len(str)
    end
    return dv ~= nil
end
local function println(str)
    local o = print(str)
    if o then
        terminal.cursor.y = terminal.cursor.y + 1
        terminal.cursor.x = 1
    end
    return o
end

function terminal.refresh(cursorOffset)
    terminal.clear()
    cursorOffset = cursorOffset or 0
    local n = cursorOffset + 1
    while n <= #terminal.lines do
        println(terminal.lines[n])
        n = n + 1
    end
    return n - 1
end

return terminal