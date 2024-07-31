 #!/bin/bash
 cd ./Release/tests/
 ./JSON_Lib_Unit_Tests
if [ $? -eq 0 ]; then
  cd ../../
else
  exit $?
fi
 cd ./Debug/tests/
 ./JSON_Lib_Unit_Tests
if [ $? -eq 0 ]; then
  cd ../../
else
  exit $?
fi

