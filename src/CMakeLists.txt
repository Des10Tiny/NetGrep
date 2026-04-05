# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g -O1")

add_executable(client client/main.cpp client/client.cpp)
# add_executable(server server/main.cpp server/server.cpp)
# add_executable(statistics statistics/main.cpp)