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
//    static std::map<uint, Interpreter> global_interpreter_pool;
    static Logger logger;

    static void createDefaultInterpreter(uint batch_size)
    {
        pDefault_interpreter = std::make_shared<Interpreter>(batch_size);
//        auto b = std::shared_ptr<int>(a, &a->y);
//        Interpreter * pDef = new Interpreter(batch_size);
//        pDefault_interpreter = pDef;
    }

    static bool isDefaultInterpreterOff(){return pDefault_interpreter == nullptr;};
    static std::shared_ptr<Interpreter> pDefault_interpreter;
};
