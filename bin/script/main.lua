-- lua script
_ENV = require("luaftpserver")

local ftpserver = ftpserver.new("0.0.0.0", 2121)

ftpserver:addUserAnonymous("anonymous", 511)
ftpserver:addUser("user",   "pass", "root", 511)

ftpserver:start(4)
ftpserver:run()
