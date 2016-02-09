# install c++ compiler
sudo apt-get install g++
sudo apt-get install cmake

# download SFML source
wget http://www.sfml-dev.org/download/sfml/2.1/SFML-2.3-sources.zip
unzip SFML-2.3-sources.zip -d sfml-sources

# install SFML dependencies
Since GLUT depends on OpenGL and a number of other libraries, installing GLUT will trigger the dependencies needed to install everything else. For distributions derived from Debian such as Ubuntu, the installation command is
- sudo apt-get install freeglut3-dev

- sudo apt-get install libxcb-image0-dev
- sudo apt-get install libudev-dev
- sudo apt-get install libopenal-dev
- sudo apt-get install libsndfile1-dev

# build
- cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -D BUILD_SHARED_LIBS=TRUE .
- make
- sudo make install

# compile programm and run
- g++ -c main.cpp -I<sfml-install-path>/include
- g++ main.o -o sfml-app -L<sfml-install-path>/lib -lsfml-graphics -lsfml-window -lsfml-system
- export LD_LIBRARY_PATH=<sfml-install-path>/lib && ./sfml-app
