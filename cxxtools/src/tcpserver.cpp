/*
 * Copyright (C) 2003 Tommi Maekitalo
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * As a special exception, you may use this file as part of a free
 * software library without restriction. Specifically, if other files
 * instantiate templates or use macros or inline functions from this
 * file, or you compile this file and link it with other files to
 * produce an executable, this file does not by itself cause the
 * resulting executable to be covered by the GNU General Public
 * License. This exception does not however invalidate any other
 * reasons why the executable file might be covered by the GNU Library
 * General Public License.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "addrinfo.h"
#include "tcpserverimpl.h"
#include <cxxtools/tcpserver.h>
#include <cxxtools/log.h>
#include <memory>

log_define("cxxtools.net.tcp")

namespace cxxtools {

namespace net {

TcpServer::TcpServer()
: _impl(0)
{
    _impl = new TcpServerImpl(*this);
}


TcpServer::TcpServer(const std::string& ipaddr, unsigned short int port, int backlog)
: _impl(0)
{
    _impl = new TcpServerImpl(*this);
    std::auto_ptr<TcpServerImpl> impl(_impl);

    this->listen(ipaddr, port, backlog);

    impl.release();
}


TcpServer::~TcpServer()
{
    try
    {
        this->close();
    }
    catch(...)
    {}

    delete _impl;
}


void TcpServer::listen(const std::string& ipaddr, unsigned short int port, int backlog)
{
    this->close();
    _impl->listen(ipaddr, port, backlog);
    this->setEnabled(true);
}


const struct sockaddr_storage& TcpServer::getAddr() const
{
    return _impl->getAddr();
}


int TcpServer::getFd() const
{
    return _impl->fd();
}


SelectableImpl& TcpServer::simpl()
{
    return *_impl;
}


TcpServerImpl& TcpServer::impl() const
{
    return *_impl;
}


void TcpServer::onClose()
{
    _impl->close();
}


bool TcpServer::onWait(std::size_t msecs)
{
    return _impl->wait(msecs);
}


void TcpServer::onAttach(SelectorBase& sb)
{
    _impl->attach(sb);
}


void TcpServer::onDetach(SelectorBase& sb)
{
    _impl->detach(sb);
}

} // namespace net

} // namespace cxxtools
