#Variabili per le regole
CC = gcc
CFLAGS = -Wall 

#Regole

#Fase di generazione .exe
esame: compito.o coda.o grafi.o
	$(CC) $(CFLAGS) -o $@ $^
	del /F *.o *.out *.gch


#Fase di generazione .o
compito.o: compito.c grafi.h node.h
	$(CC) $(CFLAGS) -c $^

coda.o: coda.c coda.h 
	$(CC) $(CFLAGS) -c $^

grafi.o: grafi.c grafi.h 
	$(CC) $(CFLAGS) -c $^


clean: 
	del /F *.o *.out *.exe *.gch

.PHONY: clean