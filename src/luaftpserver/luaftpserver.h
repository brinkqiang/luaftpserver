
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __LUAFTPSERVER_H__
#define __LUAFTPSERVER_H__

#include "sol.hpp"
#include <string>
#include "libfineftp/include/fineftp/server.h"

class Cluaftpserver
{
public:
    Cluaftpserver(sol::this_state L, const std::string& ip, int port);

    bool addUserAnonymous(const std::string& strPath,
                          const fineftp::Permission permissions);
    bool addUser(const std::string& username,
                 const std::string& password, const std::string& local_root_path,
                 const fineftp::Permission permissions);

    bool start(size_t thread_count);
    bool run();
private:
    sol::state_view m_oState;

    fineftp::FtpServer m_oServer;
};

#endif // __LUAFTPSERVER_H__