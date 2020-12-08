//
// Created by kirill on 06.12.2020.
//

#pragma once

#include <map>
#include <mutex>
#include <queue>

#include "interpreter.h"
#include "logger.h"



class LibData
{
public:
    LibData()
    {
        pDefault_interpreter = nullptr;
    }
    Logger logger;


    uint connect(uint batch_size)
    {
        {

            if (LibData::isDefaultInterpreterOff())
                LibData::createDefaultInterpreter(batch_size);
        }

        static uint context = 0;
        context += batch_size;

        return context;
    }

    void process(const std::string & current_cmd, uint context)
    {

        if (!logger.is_log_on())
            logger.turn_log_on();

        if (!logger.is_print_on())
            logger.turn_print_on();

        std::string res = pDefault_interpreter->process(current_cmd, context);
        if (!res.empty())
            logger.log(res);

    }

    void createDefaultInterpreter(uint batch_size)
    {
        pDefault_interpreter = std::make_shared<Interpreter>(batch_size);
    }


    void disconnect(uint context)
    {
        std::string res = pDefault_interpreter->stop_processing(context);
        if (!res.empty())
            logger.log(res);
    }

    bool isDefaultInterpreterOff(){return pDefault_interpreter == nullptr;};
    std::shared_ptr<Interpreter> pDefault_interpreter;
};
