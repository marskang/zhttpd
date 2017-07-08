package.path = package.path ..';../?.lua';
local resp = require('service.response')
local req = {}
req.attr = {}
local responseHeader = [[HTTP/1.1 200 OK
Content-Length: %d
Content-Type: text/html
Date: Tue, 27 Jun 2017 16:09:16 GMT
Server: zhttpd

]]

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

function service(req_path,querystring,fd)
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
    else
        resp:resp_404()
    end
end