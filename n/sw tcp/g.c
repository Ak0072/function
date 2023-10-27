#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_FRAMES 100

void sender(int window_size, int total_frames) {
    int base = 0;
    int next_frame_to_send = 0;

    while (base < total_frames) {
        for (int i = next_frame_to_send; i < next_frame_to_send + window_size && i < total_frames; i++) {
            printf("Sending frame %d...\n", i);
        }

        int ack = -1;
        printf("Enter acknowledgment for the last frame sent: ");
        scanf("%d", &ack);

        if (ack < next_frame_to_send) {
            next_frame_to_send = ack + 1;
            continue; // Go-Back-N: Resend frames from the acknowledged frame onward
        }

        base = ack + 1;
        next_frame_to_send = base;
    }

    printf("All frames sent successfully!\n");
}

void receiver(int window_size, int total_frames) {
    int base = 0;

    while (base < total_frames) {
        int ack = base;
        printf("Received frame %d. Sending acknowledgment %d...\n", base, ack);
        usleep(1000000);  // Simulating a delay in acknowledgment transmission (1 second)
        base++;
    }
}

int main() {
    int window_size, total_frames;
    printf("Enter the window size: ");
    scanf("%d", &window_size);
    printf("Enter the total number of frames: ");
    scanf("%d", &total_frames);

    sender(window_size, total_frames);
    receiver(window_size, total_frames);

    return 0;
}

