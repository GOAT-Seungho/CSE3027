#include <stdio.h>
#include <stdlib.h>     /* For atoi */
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>     /* For memset, etc. */
#include <sys/types.h>
#include <netinet/in.h> /* For htnol, htons, INADDR_ANY, sockaddr_in, etc. */
#include <fcntl.h>

#define BACKLOG 10

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    int portno;
    socklen_t clilen;
    char buffer[10000], header[10000];
    memset(header, 0, sizeof(header));

    struct sockaddr_in serv_addr, cli_addr; // 소켓 통신을 위한 구조체 선언

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // IPv4 프로토콜에 연결지향형 TCP 연결 소켓 생성
    if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("ERROR, socket");
        exit(1);
    }


    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    // bzero((char *)&serv_addr, sizeof(serv_addr));  // 서버 주소를 초기화
    portno = atoi(argv[1]);                        // 포트 번호 변환
    serv_addr.sin_family = AF_INET;                // IPv4 프로토콜
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);            // 네트워크 바이트 순서대로 ip주소와 포트 번호를 넣어준다.

    // 소켓과 서버 주소를 바인딩
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("ERROR, bind");
        exit(1);
    }

    // 연결 대기열을 BACKLOG(10)개 만큼 생성
    if (listen(sockfd, BACKLOG) == -1) {
        perror("ERROR, listen");
        exit(1);
    }

    clilen = sizeof(cli_addr);
    char data[400000];
    int n = 0;
    long len;
    int fd;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(data, 0x00, sizeof(data));
        memset(header, 0x00, sizeof(header));
        // request를 받음
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        // client로부터 올 msg를 buffer에 담기 위해 초기화
        // socket으로부터 읽어서 buffer에 채워준다.
        n = read(newsockfd, buffer, 10000);
        // 확인을 위한 프린트
        printf("%s\n", buffer);
        // 요청한 request msg에서 index를 파악하기 위해 사용
        char *tok = strtok(buffer, " ");
        tok = strtok(NULL, " ");

        if (strcmp(tok, "/index.html") == 0) {
            snprintf(header, sizeof(header), 
            "HTTP/1.0 200 OK\r\n"
            "Content-Length: 10000\r\n"
            "Content-Type: text/html\r\n"
            "\r\n");

            snprintf(data, sizeof(data),
                "<!DOCTYPE html>\n"
                "<html>\n"
                "<head>\n"
                    "<meta charset='utf-8'>\n"
                "</head>\n"
                "<body>\n"
                    "<h1>[CSE3027] Computer Networks</h1>\n"
                    "<h2>2018044411</h2>\n"
                    "<h2>소프트웨어학부 김승호</h2>\n"
                    "<a href=\"./jpeg\">JPEG</a><br/>\n"
                    "<a href=\"./gif\">GIF</a><br/>\n"
                    "<a href=\"./mp3\">MUSIC</a><br/>\n"
                    "<a href=\"./pdf\">PDF</a><br/>\n"
                "</body>\n"
                "</html>\n"
                "\r\n");
        }

        else if (strcmp(tok, "/jpeg") == 0) {
            int fd = open("./cat-min.jpeg", O_RDWR);
            len = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            read(fd, data, sizeof(data) - 1);

            snprintf(header, sizeof(header),
                "HTTP/1.0 200 OK\r\n"
                // "Accept-Ranges: bytes\r\n"
                "Content-Length: %ld\r\n"
                "Content-Type: image/jpeg\r\n"
                "Connetection: keep-alive\r\n"
                "\r\n"
            , len); // Header 작성 : Content-Length는 보내줄 파일의 크기만큼 설정
        }
        
        else if (strcmp(tok, "/gif") == 0) {
            int fd = open("./meme.gif", O_RDWR);
            len = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            read(fd, data, sizeof(data) - 1);

            snprintf(header, sizeof(header),
                "HTTP/1.0 200 OK\r\n"
                "Content-Length: %ld\r\n"
                "Content-Type: image/gif\r\n"
                // "Connection: Keep-Alive\r\n"
                "\r\n"
            , len);
        }

        else if (strcmp(tok, "/pdf") == 0) {
            int fd = open("./sample.pdf", O_RDWR);
            len = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            read(fd, data, sizeof(data) - 1);
            
            snprintf(header, sizeof(header), 
                "HTTP/1.0 200 OK\r\n"
                "Content-Length: %ld\r\n"
                "Content-Type: application/pdf\r\n"
                "\r\n"
            , len);
        }

        else if (strcmp(tok, "/mp3") == 0) {
            int fd = open("./aquarium.mp3", O_RDWR);
            len = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            read(fd, data, sizeof(data) - 1);

            snprintf(header, sizeof(header),
                "HTTP/1.0 200 OK\r\n"
                "Content-Length: %ld\r\n"
                "Content-Type: audio/mpeg\r\n"
                "Connection: Keep-Alive\r\n"
                "\r\n"
            , len);
        }
        else if (strcmp(tok, "/favicon.ico") == 0) {
            int fd = open("./favicon.ico", O_RDWR);
            len = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            read(fd, data, sizeof(data) - 1);

            snprintf(header, sizeof(header),
                "HTTP/1.0 200 OK\r\n"
                "Content-Length: %ld\r\n"
                "Content-Type: image/x-icon\r\n"
                "\r\n"
            , len);
        }

        // Header를 보내고 바로 data를 보내주었다.
        if (write(newsockfd, header, strlen(header)) > 0) {
            printf("Header send end. \n");
        }

        if (write(newsockfd, data, sizeof(data)) > 0) {
            printf("Data send end.\n\n");
        }
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}