local libservice = require "libservice"
resp = {}
resp.content = ""

resp.responseHeader = [[HTTP/1.1 200 OK
Content-Type: %s
Server: zhttpd

]]

function resp:write(content)
    self.content = self.content .. content
end

function resp:init(fd) 
    self.fd = fd
end

function resp:setContentType(contentType)
    self.contentType = contentType
end

function resp:flush()
    if self.contentType ~= nil then
        responseHeader = string.format(self.responseHeader, self.contentType)
    else 
        responseHeader = string.format(self.responseHeader, 'text/html')
    end
    libservice.resp_write(self.fd, responseHeader)
    libservice.resp_write(self.fd, self.content)
end

function resp:resp_404()
    libservice.resp_404(self.fd)
end

return resp