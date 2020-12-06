//
// Created by kirill on 06.12.2020.
//
#include "../include/libdata.h"

std::map<uint, Interpreter> LibData::global_interpreter_pool = {};
Logger LibData::logger;

std::shared_ptr<Interpreter> LibData::pDefault_interpreter = nullptr;