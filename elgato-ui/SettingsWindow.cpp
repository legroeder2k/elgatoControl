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

#include "SettingsWindow.h"
#include "ElgatoClient.h"
#include <functional>
#include "../Config.h"

SettingsWindow::SettingsWindow(std::shared_ptr<std::vector<std::shared_ptr<RemoteFixture>>>& fixtures, std::shared_ptr<ElgatoClient>& elgatoClient)
    : _fixtures(fixtures), _elgatoClient(elgatoClient)
{

    set_icon_from_file(std::string(CMAKE_INSTALL_PREFIX) + "/share/elgatoControl/idea.png");
    set_title("Elgato UI");
    set_size_request(400, 200);

    _boxLayout.set_orientation(Gtk::Orientation::ORIENTATION_VERTICAL);
    _boxLayout.set_spacing(2);
    _boxLayout.set_homogeneous();

    add(_boxLayout);

    _elgatoClient->registerCallback(sigc::mem_fun(*this, &SettingsWindow::onFixtureChanged));
}

void SettingsWindow::refreshFixtures() {
    // remove old items
    for(auto& item : _children) {
        item->remove();
    }
    _children.clear();

    for(auto& fix : *_fixtures) {
        auto item = std::make_shared<FixtureItem>(fix, _elgatoClient);
        item->show_all();

        _boxLayout.pack_end(*item, true, true);
        _children.push_back(item);
    }

    _boxLayout.show_all();

    set_size_request(400, (20 + ((int)_fixtures->size()*100)));
}

void SettingsWindow::onFixtureChanged(const FixtureUpdateEventArgs& args) {
#ifdef DEBUG_BUILD
    std::cerr << "Fixture Update for " << args.fixtureName() << " prop " << args.propertyName() << " is now " << args.newValue() << std::endl;
#endif

    for(auto& fixtureItem : _children) {
        if (fixtureItem->fixtureName() == args.fixtureName())
            fixtureItem->refreshFromData(args);
    }
}

FixtureItem::FixtureItem(std::shared_ptr<RemoteFixture>& fixture, std::shared_ptr<ElgatoClient>& client)
    : _fixture(fixture), _client(client)
{
    set_size_request(380, 90);

    _fixedLayout.set_halign(Gtk::Align::ALIGN_FILL);
    _fixedLayout.set_valign(Gtk::Align::ALIGN_FILL);

    // Title Label (Name of the fixture)
    _titleLabel.set_label(_fixture->_displayName);
    _titleLabel.set_alignment(Gtk::Align::ALIGN_START, Gtk::Align::ALIGN_CENTER);
    _titleLabel.set_size_request(380, 25);
    _fixedLayout.add(_titleLabel);
    _fixedLayout.move(_titleLabel, 70, 10);

    // Power toggle Button
    _powerButton.set_size_request(40, 20);
    _powerButton.set_active(_fixture->_powerState);
    _powerButton.property_active().signal_changed().connect(sigc::mem_fun(*this, &FixtureItem::onPowerToggle));
    _fixedLayout.add(_powerButton);
    _fixedLayout.move(_powerButton, 10, 10);

    // Color Temperature Label
    _colorTempLabelFront.set_label("Color Temp:");
    _colorTempLabelFront.set_alignment(Gtk::Align::ALIGN_START, Gtk::Align::ALIGN_CENTER);
    _colorTempLabelFront.set_size_request(120, 25);
    _fixedLayout.add(_colorTempLabelFront);
    _fixedLayout.move(_colorTempLabelFront, 10, 45);

    // Color Temperature Slider
    _colorTemp.set_range(2900, 7000);
    _colorTemp.set_digits(0);
    _colorTemp.set_value(_fixture->_temperature);
    _colorTemp.set_size_request(280, 25);
    _colorTemp.set_value_pos(Gtk::PositionType::POS_RIGHT);
    _colorTemp.signal_value_changed().connect(sigc::mem_fun(*this, &FixtureItem::onColorTempChanged));
    _fixedLayout.add(_colorTemp);
    _fixedLayout.move(_colorTemp, 120, 45);

    // Brightness Label
    _brightLabel.set_label("Brightness:");
    _brightLabel.set_alignment(Gtk::Align::ALIGN_START, Gtk::Align::ALIGN_CENTER);
    _brightLabel.set_size_request(120, 25);
    _fixedLayout.add(_brightLabel);
    _fixedLayout.move(_brightLabel, 10, 70);

    // Brightness Slider
    _brightness.set_range(0, 100);
    _brightness.set_digits(0);
    _brightness.set_value(_fixture->_brightness);
    _brightness.set_size_request(280, 25);
    _brightness.set_value_pos(Gtk::PositionType::POS_RIGHT);
    _brightness.signal_value_changed().connect(sigc::mem_fun(*this, &FixtureItem::onBrightnessChanged));
    _fixedLayout.add(_brightness);
    _fixedLayout.move(_brightness, 120, 70);

    _fixedLayout.show_all();

    add(_fixedLayout);
}

void FixtureItem::onPowerToggle() {
    if (_inUpdateFromServer) return;

    _inUpdateFromUi = true;
    if (_fixture->_powerState) {
        if (_client->powerOff(_fixture->_name)) {
            _fixture->_powerState = false;
        }
    }
    else {
        if (_client->powerOn(_fixture->_name)) {
            _fixture->_powerState = true;
        }
    }
    _inUpdateFromUi = false;
}

void FixtureItem::onBrightnessChanged() {
    if (_inUpdateFromServer) return;

    _inUpdateFromUi = true;
    auto newValue = (int32_t)std::round(_brightness.get_value());

    if (_client->setBrightness(_fixture->_name, newValue)) {
        _fixture->_brightness = newValue;
    }
    _inUpdateFromUi = false;
}

void FixtureItem::onColorTempChanged() {
    if (_inUpdateFromServer) return;

    _inUpdateFromUi = true;
    auto newValue = (int32_t)std::round(_colorTemp.get_value());

    if (_client->setColorTemp(_fixture->_name, newValue)) {
        _fixture->_temperature = newValue;
    }
    _inUpdateFromUi = false;
}

void FixtureItem::refreshFromData(const FixtureUpdateEventArgs& args) {
    if (_inUpdateFromUi) return;
    _inUpdateFromServer = true;

    if (args.propertyName() == "Power") {
        _fixture->_powerState = args.newValue() == 1;
        _powerButton.set_active(args.newValue() == 1);
    }

    if (args.propertyName() == "Brightness") {
        auto value = args.newValue();

        if (value < 0) value = 0;
        if (value > 100) value = 100;

        _fixture->_brightness = value;
        _brightness.set_value(value);
    }

    if (args.propertyName() == "Temperature") {
        auto value = args.newValue();

        if (value < 2900) value = 2900;
        if (value > 7000) value = 7000;

        _fixture->_temperature = value;
        _colorTemp.set_value(value);
    }

    _inUpdateFromServer = false;
}