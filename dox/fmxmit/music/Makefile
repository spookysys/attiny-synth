SRC	=	main_pc.c songdata.c dds.c player.c avrpc_compat.c
OBJ	=	main_pc.o songdata.o dds.o player.o avrpc_compat.o
FLAGS	=	-g -Wall
LIBS	=	-lao

all:	music

music: $(OBJ)
	gcc -o $(@) $(OBJ) $(LIBS) $(FLAGS)

clean:	
	rm -f $(OBJ) music

