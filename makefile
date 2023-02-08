CFLAGS = -lm -std=c17 -Wall

debug:
	gcc main.c tabela_hash.c -o main $(CFLAGS) -g

install:
	gcc main.c tabela_hash.c -o main $(CFLAGS)

clean:
	rm main
