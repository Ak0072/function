#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_FRAMES 100

void sender(int window_size, int total_frames) {
    int base = 0;
    int next_frame_to_send = 0;
    int ack[MAX_FRAMES] = {0}; // To keep track of acknowledgments for each frame

    while (base < total_frames) {
        for (int i = next_frame_to_send; i < next_frame_to_send + window_size && i < total_frames; i++) {
            if (ack[i] == 0) {
                printf("Sending frame %d...\n", i);
            }
        }

        int ack_frame;
        printf("Enter acknowledgment for a frame (or -1 if none): ");
        scanf("%d", &ack_frame);

        if (ack_frame >= next_frame_to_send && ack_frame < next_frame_to_send + window_size) {
            ack[ack_frame] = 1;
        }

        // Slide the window forward
        while (ack[next_frame_to_send] == 1) {
            next_frame_to_send++;
        }
    }

    printf("All frames sent successfully!\n");
}

void receiver(int window_size, int total_frames) {
    int base = 0;

    while (base < total_frames) {
        int ack_frame;
        printf("Enter acknowledgment for a frame (or -1 if none): ");
        scanf("%d", &ack_frame);

        if (ack_frame >= base && ack_frame < base + window_size) {
            printf("Received frame %d. Sending acknowledgment %d...\n", ack_frame, ack_frame);
            usleep(1000000);  // Simulating a delay in acknowledgment transmission (1 second)
        }
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

