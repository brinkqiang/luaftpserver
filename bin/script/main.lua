-- lua script
local luaftpserver = require("luaftpserver")

local ftpserver = luaftpserver.ftpserver.new("0.0.0.0", 2121)

ftpserver:addUserAnonymous("anonymous", 511)
ftpserver:addUser("user",   "pass", "root", 511)

ftpserver:start(4)
ftpserver:run()
