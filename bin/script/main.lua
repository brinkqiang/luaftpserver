-- lua script

--  enum class Permission : int
--  {
--    FileRead   = (1 << 0),  /**< Download files */
--    FileWrite  = (1 << 1),  /**< Upload files as new files */
--    FileAppend = (1 << 2),  /**< Upload files that get appended to existing files */
--    FileDelete = (1 << 3),  /**< Delete existing files or overwrite an existing file */
--    FileRename = (1 << 4),  /**< Rename existing files */
--
--    DirList    = (1 << 5),  /**< Retrieve the content of directories */
--    DirCreate  = (1 << 6),  /**< Create new directories */
--    DirDelete  = (1 << 7),  /**< Delete existing directories */
--    DirRename  = (1 << 8),  /**< Rename existing directories */
--
--    All      = (FileRead | FileWrite | FileAppend | FileDelete | FileRename | DirList | DirCreate | DirDelete | DirRename),
--    UserOnly = (FileRead | FileWrite | FileAppend | FileRename | DirList | DirCreate | DirRename),
--    ReadOnly = (FileRead | DirList),
--    None     = 0
--  };

local luaftpserver = require("luaftpserver")

local ftpserver = luaftpserver.ftpserver.new("0.0.0.0", 2121)

ftpserver:addUserAnonymous("anonymous", luaftpserver.Permission.ReadOnly )
ftpserver:addUser("user",   "user_password", "user", luaftpserver.Permission.UserOnly)
ftpserver:addUser("root",   "root_password", "root", luaftpserver.Permission.All)

ftpserver:start(4)
ftpserver:run()
