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

#include "FixtureMenuItem.h"

#include <utility>

FixtureMenuItem::FixtureMenuItem(std::shared_ptr<RemoteFixture> &fixture, std::shared_ptr<ElgatoClient> &elgatoClient)
    : _myFixture(fixture), _elgatoClient(elgatoClient)
{
    set_label(_myFixture->_displayName);

    _powerItem.set_label("Power");
    _powerItem.signal_activate().connect(sigc::mem_fun(*this, &FixtureMenuItem::onPowerToggle));

    _subMenu.append(_powerItem);

    _subMenu.show_all();
    set_submenu(_subMenu);

    signal_activate().connect(sigc::mem_fun(*this, &FixtureMenuItem::onActivateItem));
}

void FixtureMenuItem::onPowerToggle() {
    if (_inGuiUpdate) return;

    if (_myFixture->_powerState) {
        if (_elgatoClient->powerOff(_myFixture->_name))
            _myFixture->_powerState = false;

    } else {
        if (_elgatoClient->powerOn(_myFixture->_name))
            _myFixture->_powerState = true;
    }
}

void FixtureMenuItem::onActivateItem() {
    // update the power checkbox to the correct value

    _inGuiUpdate = true;
    _powerItem.set_active(_myFixture->_powerState);
    _inGuiUpdate = false;
}