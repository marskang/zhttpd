test = {}
function test.servive(req, resp)
    resp:setContentType('application/json;charset=utf-8')
    json = [[{"name" : "json", "age" : "don't konw", "国籍":"中国", "call":"hello"}]]
    resp:write(json)
end
return test