//
// Created by kirill on 25.11.2020.
//

#pragma once

#include "string"
#include "memory"
#include "commandstorage.h"



class Interpreter {
public:
    const std::string open_bracket = "{";
    const std::string close_bracket = "}";
    enum Mode {
        Static,
        Dynamic
    };

    Interpreter(uint batch_size=1);
    std::string process(const std::string & current_cmd);
    void change_mode(Mode new_mode);
    std::string stop_processing();

private:
    uint _batch_size;
    uint _bracket_counter;
    Interpreter::Mode _mode;
    CommandStorage _scb;

};