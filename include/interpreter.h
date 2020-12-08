//
// Created by kirill on 25.11.2020.
//

#pragma once
#include <map>
#include "string"
#include "memory"
#include "../include/commandstorage.h"



class Interpreter {
public:
    const std::string open_bracket = "{";
    const std::string close_bracket = "}";
    enum Mode {
        Static,
        Dynamic
    };

    Interpreter(uint batch_size=1);
    std::string process(const std::string & current_cmd, uint context = 0);
    void change_mode(Mode new_mode);
    std::string stop_processing(uint context = 0);

private:
    uint _batch_size;
    uint _bracket_counter;
    Interpreter::Mode _mode;
    CommandStorage _scb;
    std::map<uint, uint> _bracket_counters;
    std::map<uint, CommandStorage> command_storages;

};