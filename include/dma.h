#pragma once
#include <defs.h>

void dma_start(u8 start);
void dma_tick();

bool dma_transferring();