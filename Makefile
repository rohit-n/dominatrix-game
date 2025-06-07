all: src/code/game/game.so src/code/game2015/game.so

src/code/game/game.so:
	cd src/code/game/ && make -f ../../../gamedll/Makefile

src/code/game2015/game.so:
	cd src/code/game2015/ && make -f ../../../gamedll/Makefile

clean:
	cd src/code/game/ && make -f ../../../gamedll/Makefile clean
	cd src/code/game2015/ && make -f ../../../gamedll/Makefile clean
