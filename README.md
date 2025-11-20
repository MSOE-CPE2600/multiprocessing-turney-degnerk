# System Programming Lab 11 Multiprocessing

## Implementation Overview
In this lab, I implemented a multiprocessing version of the mandel() function we were given to create a mandel movie. The program created a specified number of child processses to compute frames for a video. I began by setting a max number of child processes. The child processes were created, and each child process created a frame and exited. The partial image results were aggregated into the final video using the command `ffmpeg -framerate 10 -i mandel%d.jpeg -c:v libx264 -pix_fmt yuv420p mandelmovie.mp4`. For the runtime results graph, I recorded the time from frame creation to the end of video creation. Every video was 50 frames, ensuring experimental accuracy.

## Runtime Results Graph
<img width="1817" height="993" alt="mandel_graph" src="https://github.com/user-attachments/assets/5c1462e0-b225-4ec6-944c-247e8612d967" />


## Result Discussion
As I expected when I started the lab, as the number of child processes increased, the time that it took to run the mandel program for 50 iterations decreased. As demonstarted in the graph, one max process took 882.567 seconds and 20 max processes took 131.757 seconds. 

Up until about 10 processes, doubling the processes roughly halved the runtime. This behavior aligned with parallel scaling. However, after 10 processes, the runtime improvements began to level off. This likely occurred due to scheduling constraints.

Overall, these results confirmed that multiprocessing provides a substantial speedup if you have multiple cores.

## Mandel Video
The mandel video is available in the Canvas submission. I have also uploaded it at the following Youtube link: https://www.youtube.com/shorts/Cu02bMgZtJw.