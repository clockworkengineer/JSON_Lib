 #!/bin/
cd build/tests/
./JSON_Lib_Unit_Tests
if [ $? -eq 0 ]; then
  cd ../../
else
  exit $?
fi

