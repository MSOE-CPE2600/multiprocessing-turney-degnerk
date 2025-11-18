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
    double end_scale   = 0.0001;
    double zoom_factor = pow(end_scale / start_scale, (double)frame_number / NUM_FRAMES);
    double scale = start_scale * zoom_factor;
    int max_iter = 4000;

    char command[256];
    sprintf(command, "./mandel -t %d -x %f -y %f -m %d -s %f -o mandel%d.jpeg", num_threads, x, y, max_iter, scale, frame_number);
    system(command);
}


void main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: Must use ./mandelmovie <number_of_processes> <number of threads>\n");
        exit(1);
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0 || num_processes > 50) {
        printf("Error: Number of processes must be a positive integer between 1 and 50.\n");
        exit(1);
    }

    int num_threads = atoi(argv[2]);
    if (num_threads <= 0 || num_threads > 20) {
        printf("Error: Number of threads must be a positive integer between 1 and 20.\n");
        exit(1);
    }

    // For each process number until the maximum, fork and generates one frame (and refork) until all frames are generated
    for (int i = 0; i < NUM_FRAMES; i += num_processes) {
        pid_t pids[num_processes];
        for (int j = 0; j < num_processes && (i + j) < NUM_FRAMES; j++) {
            pids[j] = fork();
            if (pids[j] < 0) {
                perror("Fork failed");
                exit(1);
            } else if (pids[j] == 0) {
                generate_frame(i + j, num_threads);
                exit(0);
            }
        }

        for (int j = 0; j < num_processes && (i + j) < NUM_FRAMES; j++) {
            wait(NULL);
        }
    }

    system("ffmpeg -framerate 10 -i mandel%d.jpeg -c:v libx264 -pix_fmt yuv420p mandelmovie.mp4");
}