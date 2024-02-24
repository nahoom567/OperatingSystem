#pragma once
#include "debug.h"
#include "assert.h"
#include "io/io.h"
#include "io/time.h"
#include "printText.h"
#include "typedefs.h"
#include "sysCalls.h"

#ifdef DEBUG_MODE
#define IF_DEBUG(action) action
#else 
#define IF_DEBUG(action) 
#endif