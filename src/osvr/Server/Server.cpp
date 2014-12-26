/** @file
    @brief Implementation

    @date 2014

    @author
    Ryan Pavlik
    <ryan@sensics.com>
    <http://sensics.com>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include <osvr/Server/Server.h>
#include <osvr/Connection/Connection.h>
#include "ServerImpl.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace server {

    ServerPtr Server::createLocal() {
        connection::ConnectionPtr conn(
            connection::Connection::createLocalConnection());
        ServerPtr ret(make_shared<Server>(conn, private_constructor{}));
        return ret;
    }

    ServerPtr Server::create(connection::ConnectionPtr const &conn) {
        ServerPtr ret(make_shared<Server>(conn, private_constructor{}));
        return ret;
    }

    void Server::start() { m_impl->start(); }

    void Server::startAndAwaitShutdown() { m_impl->startAndAwaitShutdown(); }

    void Server::stop() { m_impl->stop(); }

    void Server::signalStop() { m_impl->signalStop(); }

    void Server::loadPlugin(std::string const &plugin) {
        m_impl->loadPlugin(plugin);
    }

    void Server::instantiateDriver(std::string const &plugin,
                                   std::string const &driver,
                                   std::string const &params) {
        m_impl->instantiateDriver(plugin, driver, params);
    }

    void Server::triggerHardwareDetect() { m_impl->triggerHardwareDetect(); }

    void Server::registerMainloopMethod(MainloopMethod f) {
        m_impl->registerMainloopMethod(f);
    }

    void Server::addRoute(std::string const &routingDirective) {
        m_impl->addRoute(routingDirective);
    }

    Server::Server(connection::ConnectionPtr const &conn,
                   private_constructor const &)
        : m_impl(new ServerImpl(conn)) {}

    Server::~Server() {}

} // namespace server
} // namespace osvr