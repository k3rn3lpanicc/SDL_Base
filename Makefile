run :
	gcc main.c gbase/graphic.c -lSDL2 -lSDL2_image -lSDL2_ttf -o main;./main
	# add your other .c files to the end of the line above
	# add -lSDL2_mixer for audio support