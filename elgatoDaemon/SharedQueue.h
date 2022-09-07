/*
 * Copyright (c) 2022, Sascha Huck <sascha@wirrewelt.de>
 *
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

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SharedQueue {
public:
    SharedQueue() { };
    ~SharedQueue() { };

    T& front() {
        std::unique_lock<std::mutex> mlock(_mutex);
        while(_queue.empty())
        {
            _cond.wait(mlock);
        }

        return _queue.front();
    };

    void pop_front() {
        std::unique_lock<std::mutex> mlock(_mutex);
        while(_queue.empty()) {
            _cond.wait(mlock);
        }

        _queue.pop_front();
    }

    void push_back(const T& item) {
        std::unique_lock<std::mutex> mlock(_mutex);
        _queue.push_back(item);
        mlock.unlock();
        _cond.notify_one();
    }

    void push_back(T&& item) {
        std::unique_lock<std::mutex> mlock(_mutex);
        _queue.push_back(std::move(item));
        mlock.unlock();
        _cond.notify_one();
    }

    int size() {
          std::unique_lock<std::mutex> mlock(_mutex);
          auto size = _queue.size();
          mlock.unlock();

          return size;
    }

    bool empty() {
        return size() == 0;
    }

private:
    std::deque<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
};