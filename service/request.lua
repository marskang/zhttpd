local resp = require "libresp"
local request = {}
request.attr = {}

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

function set_querystring(querystring,fd)
    local attr_tab = split(querystring, "&")
    local body = [[<html><head><title>hello</title></head><body>what</body></html>]]
    print(body)
    print(string.len(body))
    local header = 
        string.format(responseHeader,48)
    --print(header)
    local res = resp.resp_write(fd, header)
    res = resp.resp_write(fd, body)
    print(res)
    print("fd"..fd)
    for k, v in pairs(attr_tab) do
        local t = split(v, "=")
        request.attr[t[1]] = t[2]
    end
    for k, v in pairs(request.attr) do
        print("k:"..k.."v:"..v)
    end
end