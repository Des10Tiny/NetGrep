#pragma once

#include <string>

class SocketFD {
public:
    SocketFD();
    ~SocketFD();

    SocketFD(const SocketFD&) = delete;
    SocketFD& operator=(const SocketFD&) = delete;

    SocketFD(SocketFD&& other) noexcept;
    SocketFD& operator=(SocketFD&& other) noexcept;

    int get() const;
    void shutdown_write();

private:
    int fd_{-1};
};

class Client {
public:
    Client(const std::string& ip, int port);

    void send_file(const std::string& file_path);
    std::string receive_result();

private:
    SocketFD sock_;

    void connect_to_server(const std::string& ip, int port);
    std::string read_file_content(const std::string& file_path) const;
};