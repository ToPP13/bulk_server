//
// Created by kirill on 25.11.2020.
//

#include "../include/interpreter.h"
#include "iostream"

Interpreter::Interpreter(uint batch_size):_batch_size(batch_size), _bracket_counter(0)
{
    _mode = Mode::Static;
}


std::string Interpreter::process(const std::string & current_cmd, uint context)
{
    if (command_storages.find(context) == command_storages.end())
        command_storages.insert({context, CommandStorage()});
    if (_bracket_counters.find(context) == _bracket_counters.end())
        _bracket_counters.insert({context, 0});

    std::string res = "";
    if (current_cmd == open_bracket)
    {
        if (_bracket_counters[context] == 0)
        {
            change_mode(Mode::Dynamic);
        }
        _bracket_counters[context] += 1;
        return res;
    }
    else if (current_cmd == close_bracket)
    {
        if (_mode == Mode::Dynamic)
        {
            if (_bracket_counters[context] > 0)
                _bracket_counters[context] -= 1;
            if (_bracket_counters[context] == 0)
            {
                change_mode(Mode::Static);
                res= command_storages[context].get_bulk();
                command_storages[context].clear();
            }
        }
        return res;
    }
    else
    {
        if (_mode == Mode::Dynamic)
            command_storages[context].add_command(current_cmd);
        else
        {
            _scb.add_command(current_cmd);

            if (_scb.size() == _batch_size)
            {
                res = _scb.get_bulk();
                _scb.clear();
                return res;
            }
        }

        return res;
    }
}

void Interpreter::change_mode(Mode new_mode)
{
    if (_mode == new_mode) {
        return;
    }
    _mode = new_mode;
    if (_mode == Mode::Static)
    {
        _bracket_counter = 0;
    }
}

std::string Interpreter::stop_processing(uint context)
{
    // if static - ok
    // if dynamic -
    if (command_storages.find(context) != command_storages.end())
        command_storages.erase(context);
    if (_bracket_counters.find(context) != _bracket_counters.end())
        _bracket_counters.erase(context);

    std::string res;

    if (_bracket_counters.empty())
    {
        if (!_scb.empty())
            res = _scb.get_bulk();

        _scb.clear();
    }

    return res;
}
