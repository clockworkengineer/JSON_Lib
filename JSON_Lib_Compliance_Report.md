# JSON_Lib Standard Compliance Analysis

## Overview
This document analyzes the compliance of JSON_Lib with the official JSON standard (RFC 8259/ECMA-404) and lists any deviations or non-standard behaviors found.

## Standard Compliance
- **Parsing and Stringifying:** Supports JSON parse/stringify operations for buffers and files.
- **Encoding Support:** Handles UTF-8, UTF-16LE, and UTF-16BE encodings, as required by the standard.
- **Unicode Escapes:** Implements \uXXXX escape handling and surrogate pair validation.
- **Supported Types:** Handles JSON types: numbers (int, float, double, etc.), strings, booleans, null, arrays, and objects.

## Deviations and Non-Standard Behaviors
- **Strict BOM/File Format Handling:** Throws errors for unsupported file formats or Byte Order Marks (BOMs) not recognized. RFC 8259 allows UTF-8 without BOM, so this is stricter than the standard.
- **CRLF Normalization:** Translates CRLF line endings to LF. JSON standard allows both, so this normalization is not required.
- **C++ Type Support for Numbers:** Uses C++ types (long, long long, float, double, etc.) for numbers. JSON numbers are defined as double-precision floats in the standard; using integer types may cause non-standard behavior for large numbers.
- **Strict Unicode Error Handling:** Throws errors for "unpaired surrogate found" and "invalid escaped character." RFC 8259 requires only valid Unicode characters, but the error handling is stricter.

## Summary
JSON_Lib is largely compliant with the JSON standard, but exhibits stricter behaviors in BOM handling, line ending normalization, and number type support. These may cause deviations from the standard in edge cases.

---
*Generated on March 17, 2026 by GitHub Copilot (GPT-4.1)*
