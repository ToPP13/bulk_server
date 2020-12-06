//
// Created by kirill on 05.12.2020.
//

#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <queue>
#include <atomic>
#include <sstream>
#include <fstream>

std::string string_from_int(long num);
bool file_exists (const std::string& name);

class LogEvent
{
public:
    LogEvent(std::string msg):message(msg){};
    void print() {
        std::cout << message << std::endl;
    }
    bool dump_file();
    std::string message;
};




class Logger
{
public:
    Logger();
    ~Logger();

    void log(const std::string log_data);


    bool is_print_on(){return printOn;};

    void turn_print_on();

    bool is_log_on(){return logOn;};

    void turn_log_on();

    void turn_print_off();

    void turn_log_off();

private:

    void _real_print();
    void _real_log();

    std::thread m_printThread;
    std::vector<std::thread> m_log_threads;

    std::atomic<bool> printOn = {};
    std::atomic<bool> logOn = {};

    std::mutex mutex_print;
    std::mutex mutex_log;

    std::queue<LogEvent> print_queue;
    std::queue<LogEvent> log_queue;

    uint thread_num;
};

