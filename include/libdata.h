//
// Created by kirill on 06.12.2020.
//

#pragma once

#include "interpreter.h"
#include <map>
#include <queue>
#include <mutex>
#include <../include/logger.h>



class LibData
{
public:
    static std::map<uint, Interpreter> global_interpreter_pool;
    static Logger logger;

    static void createDefaultInterpreter(uint batch_size)
    {
        Interpreter * pDef = new Interpreter(batch_size);
        pDefault_interpreter = std::shared_ptr<Interpreter>(pDef);
    }

    static std::shared_ptr<Interpreter> pDefault_interpreter;
};
