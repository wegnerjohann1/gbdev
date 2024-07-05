gb: 
	gcc .\lib\gb.c .\lib\bus.c .\lib\cart.c .\lib\cpu.c .\lib\emu.c .\lib\instructions.c -o gb -I.\include "-lraylib" -Wall

test:
	gcc test.c -o test -I.\include "-lraylib" -Wall