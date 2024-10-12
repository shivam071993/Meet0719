#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PACKET_SIZE 1024

void send_packet(const char *ip, int port, int interval) {
    int sockfd;
    struct sockaddr_in server_addr;
    char packet[PACKET_SIZE];

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    while (1) {
        // Fill packet with data
        snprintf(packet, sizeof(packet), "Hello from C program!");

        // Send packet
        if (sendto(sockfd, packet, strlen(packet), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Packet send failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Packet sent to %s:%d\n", ip, port);
        sleep(interval); // Wait for the specified interval
    }

    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <Port> <Interval in seconds>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int interval = atoi(argv[3]);

    send_packet(ip, port, interval);

    return EXIT_SUCCESS;
}
