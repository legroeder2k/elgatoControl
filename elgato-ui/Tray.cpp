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

Tray::Tray() {
    set(Gtk::Stock::JUMP_TO);

    quitItem.set_label("Quit");
    quitItem.signal_activate().connect(sigc::mem_fun(*this, &Tray::on_quitItem_activated));

    menu.append(quitItem);
    menu.show_all();

    signal_activate().connect(sigc::mem_fun(*this, &Tray::on_statusicon_activated));
    signal_popup_menu().connect(sigc::mem_fun(*this, &Tray::on_statusicon_popup));

    set_visible(true);
}

Tray::~Tray() = default;

void Tray::on_statusicon_popup([[maybe_unused]] guint button, [[maybe_unused]] guint activate_time) {
    menu.popup(button, activate_time);
}

void Tray::on_statusicon_activated() {
    std::cerr << "activated!" << std::endl;
}

void Tray::on_quitItem_activated() {
    Gtk::Main::quit();
}