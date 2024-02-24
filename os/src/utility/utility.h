#pragma once
#include "debug.h"
#include "assert.h"
#include "io.h"
#include "printText.h"
#include "typedefs.h"

#ifdef DEBUG_MODE
#define IF_DEBUG(action) action
#else 
#define IF_DEBUG(action) 
#endif