
#include <iostream>
#include <thread>
#include <string>
#include <fineftp/server.h>

#include "dmutil.h"

int main()
{
    std::string local_anonymous = DMGetRootPath() + PATH_DELIMITER_STR +
                                  "anonymous";
    DMCreateDirectories(local_anonymous);

    std::string local_user = DMGetRootPath() + PATH_DELIMITER_STR + "user";
    DMCreateDirectories(local_user);

    std::string local_root = DMGetRootPath() + PATH_DELIMITER_STR + "root";
    DMCreateDirectories(local_root);

    // Create an FTP Server on port 2121. We use 2121 instead of the default port
    // 21, as your application would need root privileges to open port 21.
    fineftp::FtpServer server(2121);

    // Add the well known anonymous user and some normal users. The anonymous user
    // can log in with username "anonyous" or "ftp" and any password. The normal
    // users have to provide their username and password.
    server.addUserAnonymous(local_anonymous, fineftp::Permission::ReadOnly);
    server.addUser         ("user", "user_password", local_user, fineftp::Permission::UserOnly);
    server.addUser         ("root", "root_password", local_root, fineftp::Permission::All);

    // Start the FTP server with 4 threads. More threads will increase the
    // performance with multiple clients, but don't over-do it.
    server.start(4);

    // Prevent the application from exiting immediatelly
    server.run();

    return 0;
}
