#include "server.hpp"
#include "client.hpp"
#include "shared_stats.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int get_free_port() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 8080;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0;

    bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

    socklen_t len = sizeof(addr);
    getsockname(sock, reinterpret_cast<struct sockaddr*>(&addr), &len);

    int port = ntohs(addr.sin_port);
    close(sock);

    return port;
}

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        port_ = get_free_port();
        Server::is_running = true;

        server_thread_ = std::thread([this]() {
            try {
                Server server(port_, "tests/test_server/data/config.txt");
                server.start();
            } catch (const std::exception& e) {
                std::cerr << "Server thread error: " << e.what() << "\n";
            }
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void TearDown() override {
        Server::is_running = false;

        try {
            Client dummy("127.0.0.1", port_);
        } catch (...) {
        }

        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

    int port_;
    std::thread server_thread_;
};

TEST_F(IntegrationTest, HandlesCleanFileCorrectly) {
    Client client("127.0.0.1", port_);

    client.send_file("tests/test_client/data/clean_file.txt");
    std::string result = client.receive_result();

    EXPECT_EQ(result, "OK: Clean");
}

TEST_F(IntegrationTest, HandlesInfectedFileCorrectly) {
    Client client("127.0.0.1", port_);

    client.send_file("tests/test_client/data/infected_file.txt");
    std::string result = client.receive_result();

    EXPECT_TRUE(result.find("DANGER: Infected") != std::string::npos);
}

TEST_F(IntegrationTest, StatisticsMemoryUpdatesCorrectly) {
    Client client("127.0.0.1", port_);
    client.send_file("tests/test_client/data/infected_file.txt");
    client.receive_result();

    SharedStats stats(2, false);

    EXPECT_EQ(stats.get_files_checked(), 1);
}