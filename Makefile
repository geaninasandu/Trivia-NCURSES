build: trivia.c
	gcc -Wall trivia.c game.c helpers.c questions.c menu.c scoreboard.c -o trivia -lncurses

run: trivia questions.txt scoreboard.txt
	./trivia questions.txt scoreboard.txt

clean:
	rm -f trivia
