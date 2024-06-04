# luaftpserver

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[![luaftpserver](https://img.shields.io/badge/brinkqiang-luaftpserver-blue.svg?style=flat-square)](https://github.com/brinkqiang/luaftpserver)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/brinkqiang/luaftpserver/blob/master/LICENSE)
[![blog](https://img.shields.io/badge/Author-Blog-7AD6FD.svg)](https://brinkqiang.github.io/)
[![Open Source Love](https://badges.frapsoft.com/os/v3/open-source.png)](https://github.com/brinkqiang)
[![GitHub stars](https://img.shields.io/github/stars/brinkqiang/luaftpserver.svg?label=Stars)](https://github.com/brinkqiang/luaftpserver) 
[![GitHub forks](https://img.shields.io/github/forks/brinkqiang/luaftpserver.svg?label=Fork)](https://github.com/brinkqiang/luaftpserver)

## Build status
| [Linux][lin-link] | [Mac][mac-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![mac-badge]       | ![win-badge]        |

[lin-badge]: https://github.com/brinkqiang/luaftpserver/workflows/linux/badge.svg "linux build status"
[lin-link]:  https://github.com/brinkqiang/luaftpserver/actions/workflows/linux.yml "linux build status"
[mac-badge]: https://github.com/brinkqiang/luaftpserver/workflows/mac/badge.svg "mac build status"
[mac-link]:  https://github.com/brinkqiang/luaftpserver/actions/workflows/mac.yml "mac build status"
[win-badge]: https://github.com/brinkqiang/luaftpserver/workflows/win/badge.svg "win build status"
[win-link]:  https://github.com/brinkqiang/luaftpserver/actions/workflows/win.yml "win build status"

## env
install cmake

## Intro
luaftpserver
```lua
local luaftpserver = require("luaftpserver")

local ftpserver = luaftpserver.ftpserver.new("0.0.0.0", 2121)

ftpserver:addUserAnonymous("anonymous", luaftpserver.Permission.ReadOnly )
ftpserver:addUser("user",   "pass", "user", luaftpserver.Permission.UserOnly)
ftpserver:addUser("root",   "pass", "root", luaftpserver.Permission.All)

ftpserver:start(4)
ftpserver:run()

```
## Contacts

## Thanks
