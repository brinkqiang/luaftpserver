
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

#include "luaftpserver.h"
#include<iostream>

Cluaftpserver::Cluaftpserver(sol::this_state L, const std::string& ip, int port)
    : m_oState(L), m_oServer(ip, port)
{

}

bool Cluaftpserver::addUserAnonymous(const std::string& strPath,
                                     const fineftp::Permission permissions)
{
    return m_oServer.addUserAnonymous(strPath, permissions);
}

bool Cluaftpserver::addUser(const std::string& username,
                            const std::string& password, const std::string& local_root_path,
                            const fineftp::Permission permissions)
{
    return m_oServer.addUser(username, password, local_root_path, permissions);
}

bool Cluaftpserver::start(size_t thread_count)
{
    return m_oServer.start(thread_count);
}

bool Cluaftpserver::run()
{
    return m_oServer.run();
}
