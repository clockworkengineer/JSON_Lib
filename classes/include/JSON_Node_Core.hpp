#pragma once
// =====================================================
// JSON node types and variants — minimal include set.
// Use this header when you only need Node, its variant
// types, and the library interfaces.  It pulls in no
// I/O, parser, translator or stringify machinery.
// =====================================================
#include "JSON_Config.hpp"
#include "JSON_Interfaces.hpp"
#include "JSON_Hole.hpp"
#include "JSON_Number.hpp"
#include "JSON_String.hpp"
#include "JSON_Boolean.hpp"
#include "JSON_Null.hpp"
#include "JSON_Node.hpp"
#include "JSON_Object.hpp"
#include "JSON_Array.hpp"
#include "JSON_Node_Reference.hpp"
#include "JSON_Node_Creation.hpp"
#include "JSON_Node_Index.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Error.hpp"
