/*
 * Copyright (c) 2021, Sascha Huck <sascha@wirrewelt.de>
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

#include <iostream>
#include "Tray.h"


Tray::Tray(const std::shared_ptr<Channel> &channel) : _client(std::make_shared<ElgatoClient>(channel)) {
    _fixtures = std::make_shared<std::vector<std::shared_ptr<RemoteFixture>>>();
    // FIXME: Find a nicer icon :)
    set(Gtk::Stock::JUMP_TO);

    _powerOnAll.set_label("All on");
    _powerOnAll.hide();
    _powerOnAll.signal_activate().connect(sigc::mem_fun(*this, &Tray::on_powerOnAll_activated));
    _menu.append(_powerOnAll);

    _powerOffAll.set_label("All off");
    _powerOffAll.hide();
    _powerOffAll.signal_activate().connect(sigc::mem_fun(*this, &Tray::on_powerOffAll_activated));
    _menu.append(_powerOffAll);

    _refreshListItem.set_label("Refresh");
    _refreshListItem.show();
    _refreshListItem.signal_activate().connect(sigc::mem_fun(*this, &Tray::on_refreshItem_activated));
    _menu.append(_refreshListItem);

    _quitItem.set_label("Quit");
    _quitItem.show();
    _quitItem.signal_activate().connect(sigc::mem_fun(*this, &Tray::on_quitItem_activated));
    _menu.append(_quitItem);

    signal_activate().connect(sigc::mem_fun(*this, &Tray::on_statusicon_activated));
    signal_popup_menu().connect(sigc::mem_fun(*this, &Tray::on_statusicon_popup));

    set_visible(true);

    _refreshListItem.activate();
}

Tray::~Tray() = default;

void Tray::on_statusicon_popup([[maybe_unused]] guint button, [[maybe_unused]] guint activate_time) {
    _menu.popup(button, activate_time);
}

void Tray::on_statusicon_activated() {
    for(const auto& fix : *_fixtures) {
        std::cerr << "Fixture " << fix->_displayName << " is " << (fix->_powerState ? "on" : "off") << std::endl;
    }
}

void Tray::on_quitItem_activated() {
    Gtk::Main::quit();
}

void Tray::on_refreshItem_activated() {
    for(const auto& mItem : _menuItems) {
        mItem->remove();
    }

    _menuItems.clear();
    _fixtures->clear();

    for(auto& fix : _client->listFixtures()) {
        auto mItem = std::make_shared<FixtureMenuItem>(fix,_client);
        mItem->show();
        _menuItems.push_back(mItem);
        _fixtures->push_back(fix);
        _menu.insert(*mItem, 2);
    }

    if (_menuItems.empty()) {
        _powerOnAll.hide();
        _powerOffAll.hide();
    } else {
        _powerOnAll.show();
        _powerOffAll.show();
    }
}

void Tray::on_powerOnAll_activated() {
    _client->powerOn("*");
}

void Tray::on_powerOffAll_activated() {
    _client->powerOff("*");
}