#pragma once

#include "stdlib.h"
#include "../include/libdata.h"

int version();

//extern LibData global_lib_data;

uint connect(uint batch_size);

void recieve(const char * buffer, uint buffer_size, uint context);

void disconnect(uint context);
