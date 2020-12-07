#include "../include/lib.h"
#include "../include/libdata.h"
#include "version.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}


uint connect(uint batch_size)
{
    if (LibData::isDefaultInterpreterOff())
        LibData::createDefaultInterpreter(batch_size);

    if (!LibData::logger.is_log_on())
        LibData::logger.turn_log_on();

    if (!LibData::logger.is_print_on())
        LibData::logger.turn_print_on();

    static uint context = 0;
    context += batch_size;

    return context;
}


void recieve(const char * buffer, uint buffer_size, uint context)
{
    std::string command = std::string(buffer, buffer_size);

    std::string res = LibData::pDefault_interpreter->process(command, context);
    if (!res.empty())
        LibData::logger.log(res);
}


void disconnect(uint context)
{
    std::string res = LibData::pDefault_interpreter->stop_processing(context);
    if (!res.empty())
        LibData::logger.log(res);
}
