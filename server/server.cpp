#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

class Server {
private:
    int server_fd;
    int port;
    std::atomic<bool> running;
    std::thread listen_thread;
    std::vector<int> clients;
    std::mutex clients_mutex;
    
    void listenLoop() {
        fd_set readfds;
        int max_fd;
        struct timeval timeout;
        
        while (running) {
            FD_ZERO(&readfds);
            FD_SET(server_fd, &readfds);
            max_fd = server_fd;
            
            
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                for (int client_fd : clients) {
                    FD_SET(client_fd, &readfds);
                    if (client_fd > max_fd) {
                        max_fd = client_fd;
                    }
                }
            }
            
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            
            int activity = select(max_fd + 1, &readfds, nullptr, nullptr, &timeout);
            
            if (activity < 0 && errno != EINTR) {
                std::cerr << "select error: " << strerror(errno) << std::endl;
                break;
            }
            
            if (activity == 0) {
                continue;
            }
            
            if (FD_ISSET(server_fd, &readfds)) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                
                int client_fd = accept(server_fd, 
                                      (struct sockaddr*)&client_addr, 
                                      &client_len);
                
                if (client_fd < 0) {
                    std::cerr << "accept error: " << strerror(errno) << std::endl;
                    continue;
                }
                
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                std::cout << "New client connected: " << client_ip 
                         << ":" << ntohs(client_addr.sin_port) 
                         << " (fd: " << client_fd << ")" << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients.push_back(client_fd);
                }
            }
            
            std::vector<int> clients_to_remove;
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                for (int client_fd : clients) {
                    if (FD_ISSET(client_fd, &readfds)) {
                        char buffer[1024];
                        ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
                        
                        if (bytes_read <= 0) {
                            if (bytes_read == 0) {
                                std::cout << "Client disconnected (fd: " << client_fd << ")" << std::endl;
                            } else {
                                std::cerr << "recv error on fd " << client_fd 
                                         << ": " << strerror(errno) << std::endl;
                            }
                            close(client_fd);
                            clients_to_remove.push_back(client_fd);
                        } else {
                            buffer[bytes_read] = '\0';
                            std::cout << "Received from fd " << client_fd 
                                     << ": " << buffer << std::endl;
                            
                            std::string response = "You just said:\n";
                            response += buffer;
                            response += "\nI got you ma boy, chill out, the things you said ruthless, so chillout monica";
                            
                            send(client_fd, response.c_str(), response.length(), 0);
                        }
                    }
                }
                
                for (int fd : clients_to_remove) {
                    clients.erase(
                        std::remove(clients.begin(), clients.end(), fd),
                        clients.end()
                    );
                }
            }
        }
    }
    
public:
    Server(int p) : server_fd(-1), port(p), running(false) {}
    
    ~Server() {
        stop();
    }
    
    bool start() {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            std::cerr << "socket creation failed: " << strerror(errno) << std::endl;
            return false;
        }
        
        
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
            close(server_fd);
            return false;
        }
        
        
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        
        
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            std::cerr << "bind failed: " << strerror(errno) << std::endl;
            close(server_fd);
            return false;
        }
        
        
        if (listen(server_fd, SOMAXCONN) < 0) {
            std::cerr << "listen failed: " << strerror(errno) << std::endl;
            close(server_fd);
            return false;
        }
        
        std::cout << "Server listening on port " << port << std::endl;
        
        
        running = true;
        listen_thread = std::thread(&Server::listenLoop, this);
        
        return true;
    }
    
    void stop() {
        if (running) {
            running = false;
            
            if (listen_thread.joinable()) {
                listen_thread.join();
            }
            
            
            
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                for (int client_fd : clients) {
                    close(client_fd);
                }
                clients.clear();
            }
            
            if (server_fd >= 0) {
                close(server_fd);
                server_fd = -1;
            }
            
            std::cout << "Server stopped" << std::endl;
        }
    }
};

int main() {
    Server server(8080);
    
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    std::cout << "Press Enter to stop server..." << std::endl;
    std::cin.get();
    
    server.stop();
    return 0;
}