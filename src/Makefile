default: pyBoids

SFMLOptions = -I/opt/homebrew/Cellar/sfml/2.5.1_2/include/ -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system

utils.o: utils.hpp utils.cpp
	clang++ -std=c++17 -c -O3 utils.cpp -o utils.o

agent.o: agent.cpp agent.hpp
	clang++ -std=c++17 -c -O3 agent.cpp -o agent.o

pyBoids: pyBoids.cpp utils.o agent.o
	clang++ -std=c++17 $(SFMLOptions) pyBoids.cpp agent.o utils.o -o pyBoids

clean:
	rm agent.o utils.o pyBoids