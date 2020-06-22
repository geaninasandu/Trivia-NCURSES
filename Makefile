build: trivia.c
	gcc -Wall trivia.c game.c helpers.c questions.c menu.c -o trivia -lncurses

run: trivia questions.txt scoreboard.txt leaderboard.txt
	./trivia questions.txt scoreboard.txt leaderboard.txt

clean:
	rm -f trivia
