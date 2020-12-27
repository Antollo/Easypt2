cmake -A x64 -B . -DPGO=GENPROFILE
cmake --build . --config Release
.\run_pgo.bat
cmake -A x64 -B . -DPGO=USEPROFILE
cmake --build . --config Release