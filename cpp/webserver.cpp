#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <filesystem>
#include "webserver.h"

#define BUFFER_SIZE 256
#define PORT 8080

const char* http_header = "HTTP/1.1 200 Ok\r\n";

int main(int argc, char* argv[])
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;
    char buffer[BUFFER_SIZE];
    int fdimg;
    int on = 1;

    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server < 0) 
    {
        perror("socket creating failed");
        exit(1);
    }

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (bind(fd_server, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("binding failed");
        close(fd_server);
        exit(1);
    }

    if (listen(fd_server, 1) == -1)
    {
        perror("listen failed");
        close(fd_server);
        exit(1);
    }

    while(true)
    {
        fd_client = accept(fd_server, (struct sockaddr*)&client_addr, &sin_len);
        if (fd_client == -1)
        {
            perror("connection failed\n");
            continue;
        }

        printf("connection successful\n");
        if (fork() == 0)
        {
            char* method, uri[BUFFER_SIZE] = ".";
            close(fd_server);
            memset(buffer, 0, BUFFER_SIZE);
            recv(fd_client, buffer, BUFFER_SIZE, 0);
            printf("%s\n", buffer);
            method = strtok(buffer,  " \t\r\n");
            strcat(uri, strtok(NULL,  " \t\r\n"));
            printf("uri:%s\n", uri);
            std::string webpage = http_header;
            webpage +=  "Content-Type: text/html; charset=UTF-8\r\n\r\n";
            if (access(uri, R_OK) == 0)
            {
                struct stat file_stats;
                stat(uri, &file_stats);
                if(S_ISDIR(file_stats.st_mode))
                {
                    webpage +=  "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>\r\n"
                                "folder</title>\r\n</head>\r\n";
                    for (const auto & entry : std::filesystem::directory_iterator(uri))
                    {
                        webpage += entry.path().filename();
                        webpage += "<br>";
                    }
                    webpage += "</html>";
                }
                else
                {
                    printf("file exists\n");
                    webpage +=  "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>\r\n";
                    
                    char* filename = strrchr(uri, '/');
                    filename++;
                    printf("file name:%s\n", filename);
                    webpage += filename;
                    webpage +="</title>\r\n</head>\r\n";
                }
            }
            else 
            {
                printf("file doesnt exists\n");
                strcpy(uri, "./error.html");
            }
            
            std::string left = webpage;
            int sent;
            while(left.length() > 0)
            {
                sent = send(fd_client, left.c_str(), left.length(), 0);
                if (sent <= 0) 
                {
                    printf("crash!");
                    break;
                }
                printf("sent %d:%s", sent, left.substr(0, sent).c_str());
                left = left.substr(sent);
            }
            send_message(fd_client, uri);
            sleep(1);
            close(fd_client);
            printf("closing connection");
            exit(0);
        }
        close(fd_client);
    } 
    return 0;
}

void send_message(int fd, const char* image_path){

    struct stat stat_buf;

    int fdimg = open(image_path, O_RDONLY);
     
    fstat(fdimg, &stat_buf);
    int img_total_size = stat_buf.st_size;
    int block_size = stat_buf.st_blksize;

    int sent_size;

    while(img_total_size > 0 && sent_size > 0){
        if(img_total_size < block_size){
            sent_size = sendfile(fd, fdimg, NULL, img_total_size);            
        }
        else{
            sent_size = sendfile(fd, fdimg, NULL, block_size);
        }       
        printf("%d \n", sent_size);
        img_total_size = img_total_size - sent_size;
    }
    close(fdimg);
}
