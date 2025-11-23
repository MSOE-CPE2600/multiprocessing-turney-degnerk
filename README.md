# System Programming Lab 11 Multiprocessing

## Implementation Overview

#### *Number of Processes versus Runtime*
In this lab, I implemented a multiprocessing version of the mandel() function we were given to create a mandel movie. The program created a specified number of child processses to compute frames for a video. I began by setting a max number of child processes. The child processes were created, and each child process created a frame and exited. When the number of processes was less than the max amount, more child processes were created until all frames were generated. The partial image results were aggregated into the final video using the command `ffmpeg -framerate 10 -i mandel%d.jpeg -c:v libx264 -pix_fmt yuv420p mandelmovie.mp4`. For the runtime results graph, I recorded the entire run using the command `time ./mandelmovie <number_processes>`. Every video was 50 frames, ensuring experimental accuracy.

#### *Number of Processes and Threads versus Runtime*
Making some slight modifications to the mandel.c and mandelmovie.c code allowed me to use both multithreading and multiprocessing. I started by allowing mandelmovie.c to take a second argument that represented the number of threads. This number is used as an argument in mandel. To apply multithreading, I made an additional create image thread function that divides the work of the image creation into segments. This allows threads to create different pieces of the image and combine them.

I used a thread data struct to ensure each thread had the image data it needed, and I used start_row and end_row variables in this struct to determine the specific rows the thread was going to make. I used pthread_create() and pthread_join() to create and join my threads, so managing the threads was an easy process. With a minor change from the lab 11 experiment, for the runtime results table, I recorded the entire run using the command `time ./mandelmovie <number_processes> <number of threads>` for every process and thread combination using the values 1, 2, 5, 10, and 20. Every video was still 50 frames.

## Runtime Results Graph

#### *Number of Processes versus Runtime*
<img width="1817" height="976" alt="mandel" src="https://github.com/user-attachments/assets/d7f4d0b6-17af-49e1-bac5-11a9d8426393" />

#### *Number of Processes and Threads versus Runtime*
<img width="1764" height="900" alt="Picture1" src="https://github.com/user-attachments/assets/6798c79f-ca2e-4784-bd5d-5e7575054ae1" />

## Result Discussion

#### *Number of Processes versus Runtime*
As I expected when I started the lab, as the number of child processes increased, the time that it took to run the mandel program for 50 iterations decreased. As demonstarted in the graph, one max process took 286.624 seconds and 20 max processes took 48.465 seconds. 

Up until about 10 processes, doubling the processes roughly halved the runtime. This behavior aligned with parallel scaling. However, after 10 processes, the runtime improvements began to level off. This likely occurred due to scheduling constraints.

Overall, these results confirmed that multiprocessing provides a substantial speedup if you have multiple cores.

#### *Number of Processes and Threads versus Runtime*
Also as I expected, increasing the number of processes and threads sped up the program to an extent. However, I also noticed the trend that too many threads seemed to have a negative impact on runtime. The longest time was 201.616 seconds with 1 process and 20 threads. The shortest time was 31.106 seconds with 20 processes and 5 threads.

The number of processes had a bigger impact on the runtimes than the number of threads. This made sense to me because increasing the number of processes allows parallelization across more CPU cores. Unlike multiprocessing, threads share memory, so locking data slowed down the speed-up provided by them. For thread counts less than 20, there were minor speed-ups. For thread counts at 20, there was likely more overhead than the speed-up they provided, causing runtime to be increased instead of decreased.

The sweetspot that I found was at 20 processes with anywhere from 2 to 10 threads. These had runtimes less than 34 seconds. 

Overall, increasing the number of processes and threads generally sped up the program. Too many threads (>10), however, had a negative impact on runtime.

## Mandel Video

The mandel video mp4 file is available in the repository. I have also provided it here.

https://github.com/user-attachments/assets/0339f4bb-7d61-4995-a708-0ae1d6a0db68
