//
// Created by kirill on 25.11.2020.
//

#include "../include/interpreter.h"
#include "iostream"

Interpreter::Interpreter(uint batch_size):_batch_size(batch_size), _bracket_counter(0)
{
    _mode = Mode::Static;
}


std::string Interpreter::process(const std::string & current_cmd) {
    std::string res = "";
    if (current_cmd == open_bracket) {

        if (_bracket_counter == 0)
        {
            change_mode(Mode::Dynamic);
            if (!_scb.empty())
            {
                res= _scb.get_bulk();
                _scb.clear();
            }
        }
        _bracket_counter += 1;
        return res;
    }
    else if (current_cmd == close_bracket)
    {
        if (_mode == Mode::Dynamic)
        {
            if (_bracket_counter > 0)
                _bracket_counter -= 1;
            if (_bracket_counter == 0)
            {
                change_mode(Mode::Static);
                res= _scb.get_bulk();
                _scb.clear();
            }
        }
        return res;
    }
    else
    {
        _scb.add_command(current_cmd);
        if (_mode == Mode::Static && (_scb.size() == _batch_size))
        {
            res = _scb.get_bulk();
            _scb.clear();
            return res;
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

std::string Interpreter::stop_processing()
{
    // if static - ok
    // if dynamic -
    if (_mode == Mode::Static)
        return _scb.get_bulk();
    return "";
}