#include "../include/lib.h"
#include "../include/libdata.h"
#include "version.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}


std::mutex mutex_lib_data;
LibData global_lib_data;

uint connect(uint batch_size)
{
    uint context;
    {
        std::lock_guard<std::mutex> lockGuard{mutex_lib_data};
        context = global_lib_data.connect(batch_size);
    }
    return context;
}


void recieve(const char * buffer, uint buffer_size, uint context)
{
    std::string command = std::string(buffer, buffer_size);
    {
        std::lock_guard<std::mutex> lockGuard{mutex_lib_data};
        global_lib_data.process(command, context);
    }
}


void disconnect(uint context)
{
    {
        std::lock_guard<std::mutex> lockGuard{mutex_lib_data};
        global_lib_data.disconnect(context);
    }
}
