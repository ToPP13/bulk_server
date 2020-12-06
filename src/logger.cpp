//
// Created by kirill on 05.12.2020.
//

#include "../include/logger.h"




std::string string_from_int(long num)
{
    std::ostringstream s;
    s << num;
    std::string num_as_string(s.str());
    return num_as_string;
}

bool file_exists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}



bool LogEvent::dump_file()
{
    std::string _filename = "";
    bool exist = true;
    for (int i = 0; i < 1000; i++)
    {
        uint now = time(nullptr);
        std::string addition = string_from_int(1);
        _filename = "bulk" + string_from_int(now) + "_" + string_from_int(random()) + ".log";
        exist = file_exists(_filename);
        if (!exist)
            break;
    }
    if (exist)
        return false;


    std::ofstream f;
    f.open(_filename,std::ios::out);
    if(!f)
        return false;
    f << message;
    f.close();
    return true;
}


Logger::Logger()
{
    thread_num = 2;
    turn_print_on();
    turn_log_on();
}

Logger::~Logger()
{
    turn_print_off();
    turn_log_off();
}


void Logger::log(const std::string log_data)
{
    {
        std::lock_guard<std::mutex> lockGuard{mutex_log};
        log_queue.emplace(LogEvent(log_data));
    }

    {
        std::lock_guard<std::mutex> lockGuard{mutex_print};
        print_queue.emplace(LogEvent(log_data));
    }
}



void Logger::turn_print_on()
{
    printOn.store(true);
    m_printThread = std::thread(&Logger::_real_print, this);
}


void Logger::turn_log_on()
{
    logOn.store(true);
    for (uint i=0;i<thread_num;i++)
        m_log_threads.emplace_back(std::thread(&Logger::_real_log, this));
}

void Logger::turn_print_off()
{
    printOn.store(false);
    if (m_printThread.joinable())
        m_printThread.join();
}


void Logger::turn_log_off()
{
    logOn.store(false);
    for (auto & th: m_log_threads)
    {
        if (th.joinable())
            th.join();
    }
    for (uint i=0;i<m_log_threads.size();i++)
        m_log_threads.pop_back();
}


void Logger::_real_print()
{
    while(printOn)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> guard{mutex_print};
            if (print_queue.empty())
                continue;

            auto& print_event = print_queue.front();
            print_event.print();
            print_queue.pop();
        }


    }
}

void Logger::_real_log()
{
    while(logOn)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> guard{mutex_log};
            if (log_queue.empty())
                continue;

            auto& print_event = log_queue.front();
            print_event.dump_file();
            log_queue.pop();
        }
    }
}



