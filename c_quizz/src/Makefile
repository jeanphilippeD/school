OBJS	=  aquisition_dico.c aquisition_question.c callbacks.c Ma_String_Entry.c pretraitement.c pretraitement_conversion.c pretraitement_fitness.c pretraitement_rang.c pretraitement_tri.c pretraitement_un_mot.c traitementreponse.c main.c
CFLAGS = -ansi -Wall -I/usr/local/include

all : $(OBJS)
	gcc  -ansi -Wall -o Quizz $(OBJS) -L/usr/local/lib -lsx -lm
