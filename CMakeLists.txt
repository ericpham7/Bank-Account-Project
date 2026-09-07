#If your IDE does not have cmake support, 
CMAKE_MINIMUM_REQUIRED(VERSION 3.25) 

PROJECT(bankAccount)

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


ADD_EXECUTABLE(bankAccount main.cpp bankAccount.cpp display.cpp)

target_include_directories(bankAccount PRIVATE include)