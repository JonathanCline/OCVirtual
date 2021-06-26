local task = {}

-- Grab libs
local coroutine
do
    local raw_coroutine = require("coroutine")
    coroutine =
    {
        yield = raw_coroutine.yield,
        resume = raw_coroutine.resume,
        create = raw_coroutine.create,
        close = raw_coroutine.close,
        status = raw_coroutine.status
    }
end

local computer = require("computer")
local table = require("table")

local taskList = {}
local taskIDCounter = 0
local thisTaskID = 0

taskList[0] =
{
    id = 0
}

-- Makes a new, unique task ID
local function new_task_id()
    taskIDCounter = taskIDCounter + 1
    return taskIDCounter
end

-- Gets the task table from a task ID
local function get_task(_taskID)
    local _o = taskList[_taskID]
    assert(type(_o) == "table", "bad task ID")
    return _o
end

-- Returns the current task table
local function get_current_task()
    return get_task(thisTaskID)
end

-- Makes a new task, setting the parent to the current() task
function task.new(_function)
    local _id = new_task_id()
    local _task =
    {
        id = _id,
        coid = coroutine.create(_function),
        parent = get_current_task().id
    }
    return _id
end

function task.detach(_taskID)
    local _task = get_task(_taskID)
    assert(_task.parent == get_current_task().id, "cannot detach non-child task")
    _task.parent = 0
end

function task.spawn(_function)
    local _task = task.new(_function)
    local _id = _task.id
    task.detach(_id)
    return _id
end

function task.current()
    return get_current_task().id
end

function task.yield(...)
    return coroutine.yield(...)
end

function task.resume(_taskID, ...)
    local _fromID = get_current_task().id
    local _task = get_task(_taskID)
    thisTaskID = _task.id
    local _rets = table.pack(coroutine.resume(_task.coid, ...))
    thisTaskID = _fromID
    return table.unpack(_rets)
end

function task.kill(_taskID)
    local _task = get_task(_taskID)
    coroutine.close(_task.coid)
end

function task.done()
    -- Empty for now, should eventually never return
end

return task