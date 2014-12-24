##Mark-and-sweep garbage collector for [LLVM](http://llvm.org/)

Project of Bakradze Liana, Samofalov Aleksandr, Shashkova Elizaveta - students of Software engineering chair at Faculty of mathematics and mechanics of Saint Petersburg State University.



### Instruction
**1. Download and build LLVM 3.3. from sources**

You can download sources from [here](http://llvm.org/releases/download.html#3.3)

Instruction for building LLVM is [here](http://llvm.org/docs/GettingStarted.html)

**2. Clone repository and switch to the gc branch**

    git clone https://github.com/EvaGL/llvm-gc.git
    cd llvm-gc
    git checkout gc
    
**3. Copy folder llvm-compiler-plugin to the lib folder inside LLVM sources and go there**

**4. Build plugin**

    make
    make install

**5. Return to the folder containing this repository and go to the gc folder**

**6. Build gc library and tests**

    make
    make tests

**7. Append path to the library in the LD_LIBRARY_PATH enviroment variable**

    export LD_LIBRARY_PATH="$(pwd):$LD_LIBRARY_PATH" 
Write absolute path to the folder with library instead of $(pwd) if you are not in gc folder.
You should run this command every time when you are working with this garbage collector.
If you don't want to do that every time you must add this command to ~/.profile and reboot.
