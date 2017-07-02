--[[local param;
param = set_lua_request_param();
print(param); ]]--
print("hello world\n")
function fun(str)
    print(str)
end

local tab = set_table_test()
for k,v in pairs(tab) do
    print("key:" .. k)
    print("val:" .. v)
    print()
end