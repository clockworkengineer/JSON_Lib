# JSON_Lib Phase 1 Baseline

## Build and Artifact Sizes
- Library: `build\Debug\JSON_Lib.lib` = 6698.07 KB
- Benchmark/Test executable: `build\tests\Debug\JSON_Lib_Unit_Tests.exe` = 6106 KB

## Benchmark Runtime Baseline
Run command used:
- `JSON_Lib_Unit_Tests.exe [Benchmark] --benchmark-samples 20 --benchmark-no-analysis -s`

Measured results:
- Parse large numeric array: 44.9591 ms
- Stringify large numeric array: 107.534 ms
- Parse large object with many keys: 14.9367 ms
- Object key lookup in large object: 18.3994 ms
- Traverse large array JSON: 42.6624 ms

## Notes
- Benchmarks were added in `tests/source/benchmark/JSON_Lib_Tests_Benchmark.cpp`.
- Catch2 benchmarking support was enabled via `tests/include/JSON_Lib_Tests.hpp`.
- The benchmark target is now part of the existing `JSON_Lib_Unit_Tests` executable.

## Next Step
Phase 2 should start by optimizing the node representation and parser/stringifier dispatch, using this baseline for comparison.
