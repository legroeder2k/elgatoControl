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

#include "Log.h"

#include <cstring>
#include <ostream>

Log::Log(std::string ident, int facility) {
    _faciltiy = facility;
    _priority = LOG_DEBUG;
    strncpy(_ident, ident.c_str(), sizeof(_ident));
    _ident[sizeof(_ident)-1] = '\0';

    openlog(_ident, LOG_PID, _faciltiy);
}

int Log::sync() {
    if (_buffer.length()) {
        syslog(_priority, "%s", _buffer.c_str());
        _buffer.erase();
        _priority = LOG_DEBUG;
    }

    return 0;
}

int Log::overflow(int c) {
    if (c != EOF) {
        _buffer += static_cast<char>(c);
    } else {
        sync();
    }

    return c;
}

std::ostream& operator<< (std::ostream& ostream, const LogPriority& log_priority) {
    static_cast<Log*>(ostream.rdbuf())->_priority = (int)log_priority;
    return ostream;
}