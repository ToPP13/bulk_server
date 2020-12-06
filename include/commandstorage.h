//
// Created by kirill on 25.11.2020.
//

#pragma once

#include <vector>
#include <string>
#include "../include/pretty.h"


class CommandStorage {
public:
    CommandStorage() = default;

    virtual void add_command(const std::string & command);

    bool empty() const{return _commands.empty();};
    void clear(){_commands.clear();};
    uint size() const{return _commands.size();};
    std::string get_bulk() const;

    std::string _filename;
    std::vector<std::string> _commands;
};