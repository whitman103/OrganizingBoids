default: pyBoids

# Keeping the libomp flags around in case I want to implement it.
#CompilerOptions = -L/opt/homebrew/opt/libomp/lib/ -I/opt/homebrew/opt/libomp/include -lomp
CompilerOptions = 

SFMLOptions = -I/opt/homebrew/Cellar/sfml/2.5.1_2/include/ -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system

utils.o: utils.hpp utils.cpp
	clang++ -std=c++17 -c -O3 utils.cpp -o utils.o $(CompilerOptions)

agent.o: agent.cpp agent.hpp constants.hpp
	clang++ -std=c++17 -c -O3 agent.cpp -o agent.o $(CompilerOptions)

grid.o: grid.cpp agent.o grid.hpp
	clang++ -Xclang -fopenmp  -std=c++17 -c -O3 grid.cpp -o grid.o $(CompilerOptions)

pyBoids: pyBoids.cpp utils.o agent.o grid.o constants.hpp
	clang++ -std=c++17 -O3 $(SFMLOptions) pyBoids.cpp agent.o utils.o grid.o -o pyBoids $(CompilerOptions)

clean:
	rm agent.o utils.o pyBoids grid.o