/*
 * @Author: Chikee royallor@163.com
 * @Date: 2024-04-21 23:21:52
 * @LastEditors: Chikee royallor@163.com
 * @LastEditTime: 2024-05-05 22:07:39
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
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "timer_manager.hpp"

std::map<std::string, std::string> data;
TimerManager tm;

void handle_client(int client_fd)
{
    char buffer[1024] = {0};
    std::string command;
    std::string resp;

    while (true)
    {
        int len = recv(client_fd, buffer, 1024, 0);
        if (len <= 0)
        {
            std::cerr << "Failed to receive data from client\n";
            break;
        }
        int idx = 0;
        if (buffer[idx] != '*')
            continue;
        idx += 1;
        int i = idx;
        while (buffer[i] != '\r')
        {
            i++;
        }
        int num = std::stoi(std::string(buffer + idx, i - idx));
        std::cout<<"command num: "<<num<<std::endl;
        idx = i + 2;
        std::vector<std::string> commands;
        for (int i = 0; i < num; i++)
        {
            while (buffer[idx] != '$')
                idx++;
            idx += 1;
            int j = idx;
            while (buffer[j] != '\r')
            {
                j++;
            }
            int method_len = std::stoi(std::string(buffer + idx, j - idx));
            std::cout<<"method len: "<<method_len<<std::endl;
            idx = j + 2;
            std::string method = std::string(buffer + idx, method_len);
            std::cout<<"method: "<<method<<std::endl;
            idx += method_len + 2;
            commands.push_back(method);
        }
        std::string method = commands[0];
        std::cout<<method<<std::endl;
        if (method == "PING")
        {
            resp = "+PONG\r\n";
        }
        else if (method == "ECHO")
        {
            resp = "+" + commands[1] + "\r\n";
        }
        else if (method == "SET")
        {
            idx += 2;
            std::string key = commands[1];
            std::string value = commands[2];
            std::string extra_args;
            int expire = -1;
            std::cout<<commands.size()<<std::endl;
            if (commands.size() > 3)
            {
                std::string extra_args = commands[3];
                std::cout<<extra_args<<std::endl;
                if (extra_args == "px")
                {
                    expire = std::stoi(commands[4]);
                    std::cout << expire << std::endl;
                }
            }
            if (data.find(key) != data.end())
            {
                data[key] = value;
            }
            else
                data.insert(std::pair<std::string, std::string>(key, value));
            tm.addTimer(expire, [key]() { data.erase(key); });
            resp = "+OK\r\n";
        }
        else if (method == "GET")
        {
            idx += 2;
            std::string key = commands[1];
            if (data.contains(key) && tm.getNextExpire() > 0){
                resp = "+" + data[key] + "\r\n";
            }
            else
                resp = "$-1\r\n";
        }
        else if(method == "INFO"){
            
        }
        send(client_fd, resp.c_str(), resp.length(), 0);
    }
    close(client_fd);
}

int main(int argc, char** argv)
{
    // You can use print statements as follows for debugging, they'll be visible
    // when running tests.
    std::cout << "Logs from your program will appear here!\n";

    // argv
    uint16_t port = 6379;
    if(argc >= 2){
        std::cout<<argv[1]<<std::endl;
        int idx = 0;
        while(argv[1][idx] == '-') idx++;
        std::string para;
        while(argv[1][idx] != '\0'){
            para += argv[1][idx];
            idx++;
        }
        if(para == "port"){
            port = std::stoi(argv[2]);
        }
    }
    // Uncomment this block to pass the first stage

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
        0)
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) !=
        0)
    {
        std::cerr << "Failed to bind to port 6379\n";
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
    }

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    std::cout << "Waiting for a client to connect...\n";
    std::vector<std::thread> threads;
    int client_fd = -1;

    while (true)
    {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr,
                           (socklen_t*)&client_addr_len);
        if (client_fd > 0)
        {
            std::cout << "connect to client" << std::endl;
            threads.emplace_back(std::thread(handle_client, client_fd));
        }
        else
        {
            std::cerr << "Failed to accept client connection\n";
            break;
        }
    }

    for (auto& t : threads)
    {
        t.join();
    }
    close(server_fd);

    return 0;
}
