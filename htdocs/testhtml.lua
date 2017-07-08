test = {}
function test.servive(req, resp)
    resp:setContentType('text/html')
    html = [[
<html>
    <head>
        <meta charset="UTF-8">
        <title>test</title>
    </head>
    <body>
        <p>你好,世界</p>
    </body>
</html>
    ]]
    resp:write(html)
end
return test