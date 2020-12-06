#pragma once

#include "stdlib.h"

int version();

uint connect(uint batch_size);

void recieve(const char * buffer, uint buffer_size, uint context);

void disconnect(uint context);

void process_command(const char * buffer, uint buffer_size, uint batch_size);