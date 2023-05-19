# HelloWorld Pass

## build
Normal build.

    xxx@debian:~/HelloWorld$ mkdir build
    xxx@debian:~/HelloWorld$ cd build/
    xxx@debian:~/HelloWorld/build$ cmake -DCMAKE_BUILD_TYPE=Debug ../
    xxx@debian:~/HelloWorld/build$ make

## debug build
Specify debug mode in cmake, so compiler will build it with debug symbols.

    xxx@debian:~/HelloWorld/build$ cmake -DCMAKE_BUILD_TYPE=Debug ../

## run the pass
Run llvm opt, load the pass and feed it with a llir input file. The pass should print out functions defined in the ll file.

    xxx@debian:~/HelloWorld/build$ /path/to/llvm/opt -load-pass-plugin=./libHelloWorld.so -passes="HelloWorld" /path/to/llvm/test/test.ll

## debug run
Specify debug mode in cmake frist, then run debug target, it will start a gdb and run the pass.

    xxx@debian:~/HelloWorld/build$ cmake -DCMAKE_BUILD_TYPE=Debug ../
    xxx@debian:~/HelloWorld/build$ make debug
    (gdb) b HelloWorld::run
    (gdb) r
