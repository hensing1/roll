roll: main.c
	cc main.c -o roll

install: roll
	cp roll ~/.local/bin/

uninstall:
	rm ~/.local/bin/roll
