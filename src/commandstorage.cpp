//
// Created by kirill on 25.11.2020.
//

#include "../include/commandstorage.h"


using std::string;


void CommandStorage::add_command(const string & command)
{
    _commands.emplace_back(command);
}


std::string CommandStorage::get_bulk() const
{
    std::string result = "bulk: ";
    for (auto iter = _commands.begin(); iter != _commands.end(); iter++) {
        if (iter != _commands.begin())
            result += (", " + *iter);
        else
            result += *iter;
    }
    return result;
}