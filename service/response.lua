local libservice = require "libservice"
resp = {}
function resp:write(content)
    print('hello world')
    print(self.fd)
    libservice.resp_write(self.fd, content)
end

function resp:init(fd) 
    self.fd = fd
end

function resp:resp_404()
    libservice.resp_404(self.fd)
end

return resp