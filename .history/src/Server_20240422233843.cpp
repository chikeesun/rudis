/*
 * @Author: Chikee royallor@163.com
 * @Date: 2024-04-21 23:21:52
 * @LastEditors: Chikee royallor@163.com
 * @LastEditTime: 2024-04-22 23:33:42
 * @FilePath: /codecrafters-redis-cpp/src/Server.cpp
 * @Copyright (c) 2024 by Robert Bosch GmbH. All rights reserved.
 * The reproduction, distribution and utilization of this file as
 * well as the communication of its contents to others without express
 * authorization is prohibited. Offenders will be held liable for the
 * payment of damages. All rights reserved in the event of the grant
 * of a patent, utility model or design.
 */
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(6379);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) !=
      0) {
    std::cerr << "Failed to bind to port 6379\n";
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int client_fd = -1;
  client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                     (socklen_t *)&client_addr_len);
  if (client_fd > 0) {
    std::cout << "Client connected\n";
  }

  while (true) {
    if (client_fd <= 0) {
      std::cerr << "Failed to accept connection\n";
      break;
    }

    char buffer[1024] = {0};
    if (recv(client_fd, buffer, 1024, 0) < 0) {
      std::cerr << "Failed to receive data from client\n";
      break;
    }
    std::string received_data(buffer);
    std::cout << "Received data: " << received_data << std::endl;

    size_t l = 0;
    while (l < received_data.length()) {
      std::string command;
      std::cout << "Command: " << command << std::endl;

      std::string resp = "+PONG\r\n";
      send(client_fd, resp.c_str(), resp.length(), 0);
    }
  }

  close(server_fd);

  return 0;
}
