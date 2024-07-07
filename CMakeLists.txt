cmake_minimum_required(VERSION 3.6)
project (laplas)

ADD_LIBRARY(LibsModule 
    main.c
    fileReader.c
    object.c
    value.c
    scanner.c
    memory.c
    stringTable.c
    globalState.c
)

ADD_EXECUTABLE(laplas main.c)
target_link_libraries(laplas LibsModule)