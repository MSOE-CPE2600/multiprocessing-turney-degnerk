/*********************************************************************************************************************
 * Filename: mandelmovie.c
 * Name: Kadie Degner
 * Date: 11/11/2025
 * Description: This file takes a parameter with a number of child processes and creates a mandel movie with 50 frames
 * Note: Compile with gcc -o mandelmovie mandelmovie.c -lm and run with ./mandelmovie <number_of_processes>
 *********************************************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

#define NUM_FRAMES 50

void generate_frame(int frame_number, int num_threads) {
    double x = -0.74;
    double y =  0.13;

    double start_scale = 0.20;
    double end_scale   = 0.001;
    double zoom_factor = pow(end_scale / start_scale,
                             (double)frame_number / NUM_FRAMES);
    double scale = start_scale * zoom_factor;
    int max_iter = 500;

    char command[256];
    sprintf(command, "./mandel -x %f -y %f -m %d -s %f -o mandel%d.jpeg",
            x, y, max_iter, scale, frame_number);

    system(command);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Must use ./mandelmovie <number_of_processes>\n");
        exit(1);
    }

    int max_procs = atoi(argv[1]);
    if (max_procs <= 0) {
        printf("Error: Number of processes must be positive.\n");
        exit(1);
    }

    int running = 0;
    int frame = 0;

    while (frame < NUM_FRAMES || running > 0) {

        // Spawn new children if we have capacity and still have frames to generate
        while (running < max_procs && frame < NUM_FRAMES) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(1);
            }

            if (pid == 0) {
                generate_frame(frame);
                exit(0);
            }

            running++;
            frame++;
        }

        // Wait for child to finish
        wait(NULL);
        running--;
    }

    // After all frames done, make video
    system("ffmpeg -framerate 10 -i mandel%d.jpeg -c:v libx264 -pix_fmt yuv420p mandelmovie.mp4");

    return 0;
}
