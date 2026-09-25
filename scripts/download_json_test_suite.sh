#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_DIR="${SCRIPT_DIR}/../tests/JSONTestSuite"

if [ -d "${TARGET_DIR}" ]; then
  echo "JSONTestSuite already exists at ${TARGET_DIR}. Pulling latest changes..."
  git -C "${TARGET_DIR}" pull
else
  echo "Cloning official JSONTestSuite (https://github.com/nst/JSONTestSuite.git) into ${TARGET_DIR}..."
  git clone --depth 1 https://github.com/nst/JSONTestSuite.git "${TARGET_DIR}"
fi

echo "JSONTestSuite is ready. Re-run CMake to include JSONTestSuite in unit tests."
