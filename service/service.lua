package.path = package.path ..';../?.lua';
local resp = require('service.response')
local req = {}
req.attr = {}

function split(s, delim)
    if type(delim) ~= "string" or string.len(delim) <= 0 then
        return
    end
    local start = 1
    local t = {}
    while true do
    local pos = string.find (s, delim, start, true)
        if not pos then
          break
        end
        table.insert (t, string.sub (s, start, pos - 1))
        start = pos + string.len (delim)
    end
    table.insert (t, string.sub (s, start))
    return t
end

function file_exists(path)
    local file = io.open(path, "rb")
    if file then file:close() end
    return file ~= nil
end
function urlEncode(s)
    s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)
    return string.gsub(s, " ", "+")
end

function urlDecode(s)
    s = string.gsub(s, '%%(%x%x)', function(c) return string.char(tonumber(c, 16)) end)
    return s
end

function service(req_path,querystring,fd)
    querystring = urlDecode(querystring)
    local attr_tab = split(querystring, "&")
    for k, v in pairs(attr_tab) do
        local t = split(v, "=")
        req.attr[t[1]] = t[2]
    end
    resp:init(fd)
    if file_exists(req_path) then 
        module_path = string.gsub(req_path, "/", ".")
        module_path = string.gsub(module_path, ".lua", "")
        local req_module = require(module_path)
        req_module.servive(req, resp)
        resp:flush()
    else
        resp:resp_404()
    end
end