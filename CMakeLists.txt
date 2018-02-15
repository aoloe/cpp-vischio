cmake_minimum_required(VERSION 3.4)
project(vischio)

ADD_DEFINITIONS("-std=c++17 -Wall")


ADD_EXECUTABLE (vischio
    # src/filetokenizer.h
    # src/blocktoken.h
    # src/blocktokenizer.h
    # src/htmlrenderer.h
    # src/document.h
    src/main.cpp
)
