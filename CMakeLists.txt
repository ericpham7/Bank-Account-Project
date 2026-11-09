#If your IDE does not have cmake support, 
CMAKE_MINIMUM_REQUIRED(VERSION 3.25) 
PROJECT(bankAccount LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets)

ADD_EXECUTABLE(bankAccount
    main.cpp
    bankAccount.cpp
    bankingWindow.cpp
)

target_include_directories(bankAccount PRIVATE include)
target_link_libraries(bankAccount PRIVATE Qt6::Widgets)

if(APPLE)
    set_target_properties(bankAccount PROPERTIES MACOSX_BUNDLE TRUE)
endif()