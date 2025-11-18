CC = gcc
CFLAGS = -c -Wall -g
LDFLAGS = -ljpeg

SOURCES = mandel.c jpegrw.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = mandel

MOVIE_SRC = mandelmovie.c
MOVIE_OBJ = $(MOVIE_SRC:.c=.o)
MOVIE_EXEC = mandelmovie

all: $(EXECUTABLE) $(MOVIE_EXEC)

-include $(OBJECTS:.o=.d)
-include $(MOVIE_OBJ:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ -lm -lpthread

$(MOVIE_EXEC): $(MOVIE_OBJ)
	$(CC) $(MOVIE_OBJ) -o $@ -lm

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d

clean:
	rm -rf $(OBJECTS) $(MOVIE_OBJ) $(EXECUTABLE) $(MOVIE_EXEC) *.d *.jpeg *.mp4 *.jpg
