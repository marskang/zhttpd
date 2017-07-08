test = {}
function test.servive(req, resp)
    name = req.attr['name']
    html = [[
<html>
    <head>
        <meta charset="UTF-8">
        <title>test</title>
    </head>
    <body>
        <p>name:%s</p>
    </body>
</html>
    ]]
    html = string.format(html, name)
    resp:write(html)
end
return test