echo "gcd`n"

echo "`nPython"
python --version
python benchmarks/gcd.py

echo "`nEasypt"
./bin/easypt.exe --version
./bin/easypt.exe -file benchmarks/gcd.ez

echo "`nNode.js (no jit)"
node --version
node --allow-natives-syntax  benchmarks/gcd_no_jit.js

echo "`nNode.js"
node benchmarks/gcd.js

echo "`n`nsort`n"

echo "`nPython"
python --version
python benchmarks/sort.py

echo "`nEasypt"
./bin/easypt.exe --version
./bin/easypt.exe -file benchmarks/sort.ez

echo "`nNode.js (no jit)"
node --version
node --allow-natives-syntax  benchmarks/sort_no_jit.js

echo "`nNode.js"
node benchmarks/sort.js