#include <stdio.h>
#include <unistd.h>

#define MAX_FRAMES 100

void sender(int window_size, int total_frames) {
    int base = 0;
    int next_frame_to_send = 0;

    while (base < total_frames) {
        while (next_frame_to_send < base + window_size && next_frame_to_send < total_frames) {
            printf("Sending frame %d...\n", next_frame_to_send);
            next_frame_to_send++;
        }

        int ack = -1;
        while (ack < base) {
            printf("Enter acknowledgment for frame: ");
            scanf("%d", &ack);
        }

        base = ack + 1;
    }

    printf("All frames sent successfully!\n");
}

void receiver(int window_size, int total_frames) {
    int base = 0;

    while (base < total_frames) {
        int frame_to_receive = base;
        int ack = frame_to_receive;
        printf("Received frame %d. Sending acknowledgment %d...\n", frame_to_receive, ack);
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

