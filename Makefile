web:
	./emsdk_portable/emscripten/master/em++  -Wall  -O3  -I boost/  -std=c++11  -s NO_EXIT_RUNTIME=1  -s DEMANGLE_SUPPORT=1 -s ALLOW_MEMORY_GROWTH=1  -s EXPORTED_FUNCTIONS="['_cmd','_main']"  src/*.cpp  -o build/harcs.js

cpp:
	g++ -std=c++11 -Wall -O3 src/main.cpp src/engine.cpp -o build/a.out

# -s TOTAL_MEMORY=10167108864