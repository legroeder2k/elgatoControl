/*
 * Copyright (c) 2022, Sascha Huck <sascha@wirrewelt.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#pragma once

#include <gtkmm.h>
#include <unistd.h>

#include "ElgatoClient.h"
#include "FixtureMenuItem.h"

class Tray : public Gtk::StatusIcon {
public:
    explicit Tray(const std::shared_ptr<Channel>&);
    ~Tray() override;

private:
    virtual void on_statusicon_popup(guint, guint);
    virtual void on_statusicon_activated();
    virtual void on_quitItem_activated();
    virtual void on_refreshItem_activated();
    virtual void on_powerOnAll_activated();
    virtual void on_powerOffAll_activated();

    Gtk::Menu _menu;
    Gtk::MenuItem _powerOnAll;
    Gtk::MenuItem _powerOffAll;
    Gtk::MenuItem _refreshListItem;
    Gtk::MenuItem _quitItem;
    std::shared_ptr<ElgatoClient> _client;

    std::vector<std::shared_ptr<FixtureMenuItem>> _menuItems;
    std::shared_ptr<std::vector<std::shared_ptr<RemoteFixture>>> _fixtures;
};
