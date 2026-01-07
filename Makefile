all:
	 gcc -std=c23 -o shell src/main.c src/commands.c src/input.c src/signals.c src/tokenizer.c -lreadline && ./shell && rm shell

