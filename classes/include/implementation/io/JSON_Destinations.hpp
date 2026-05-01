#pragma once

#include "JSON_DestinationBase.hpp"
#include "JSON_BufferDestination.hpp"
#include "JSON_FixedBufferDestination.hpp"

#if !JSON_LIB_NO_STDIO
#include "JSON_FileDestination.hpp"
#endif