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

#include "ElgatoClient.h"

class FixtureUpdateEventArgs;

class FixtureItem : public Gtk::Frame {
public:
    FixtureItem(std::shared_ptr<RemoteFixture>&, std::shared_ptr<ElgatoClient>&);
    [[nodiscard]] std::string fixtureName() const { return  _fixture->_name; }

    void refreshFromData(const FixtureUpdateEventArgs&);

protected:
    virtual void onPowerToggle();
    virtual void onColorTempChanged();
    virtual void onBrightnessChanged();

private:
    std::shared_ptr<RemoteFixture> _fixture;
    std::shared_ptr<ElgatoClient> _client;

    volatile bool _inUpdateFromServer = false;
    volatile bool _inUpdateFromUi = false;

    Gtk::Fixed _fixedLayout;

    Gtk::Label _titleLabel;
    Gtk::Switch _powerButton;

    Gtk::Label _colorTempLabelFront;
    Gtk::Scale _colorTemp = Gtk::Scale(Gtk::Orientation::ORIENTATION_HORIZONTAL);

    Gtk::Label _brightLabel;
    Gtk::Scale _brightness = Gtk::Scale(Gtk::Orientation::ORIENTATION_HORIZONTAL);
};

class SettingsWindow : public Gtk::Window {
public:
    SettingsWindow(std::shared_ptr<std::vector<std::shared_ptr<RemoteFixture>>>&, std::shared_ptr<ElgatoClient>&);

    void refreshFixtures();
protected:

private:
    void onFixtureChanged(const FixtureUpdateEventArgs&);
    std::shared_ptr<std::vector<std::shared_ptr<RemoteFixture>>> _fixtures;
    std::shared_ptr<ElgatoClient> _elgatoClient;
    std::vector<std::shared_ptr<FixtureItem>> _children;

    Gtk::Box _boxLayout;
};
