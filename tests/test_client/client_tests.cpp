#include <gtest/gtest.h>
#include <stdexcept>
#include "client.hpp"

TEST(SocketTests, BaseCreate) {
    SocketFD sock1;
    EXPECT_GE(sock1.get(), 0);
}

TEST(SocketTests, BaseCreateAndMove) {
    SocketFD sock1;
    EXPECT_GE(sock1.get(), 0);

    SocketFD sock2(std::move(sock1));
    EXPECT_EQ(sock1.get(), -1);
    EXPECT_GE(sock2.get(), 0);
}

TEST(ClientTest, ConnectionRefused) {
    int dead_port = 77777;
    EXPECT_THROW({ Client client("127.0.0.1", dead_port); }, std::runtime_error);
}

TEST(ClientTest, InvalidIpAddress) {
    EXPECT_THROW({ Client client("777.777.777.777", 8080); }, std::runtime_error);
}