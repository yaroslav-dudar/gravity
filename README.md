# install c++ compiler
sudo apt-get install g++
sudo apt-get install cmake

# download SFML source
- wget http://www.sfml-dev.org/download/sfml/2.1/SFML-2.3-sources.zip
- unzip SFML-2.3-sources.zip -d sfml-sources

# install SFML dependencies
Since GLUT depends on OpenGL and a number of other libraries, installing GLUT will trigger the dependencies needed to install everything else. For distributions derived from Debian such as Ubuntu, the installation command is
- sudo apt-get install freeglut3-dev

- sudo apt-get install libxcb-image0-dev
- sudo apt-get install libudev-dev
- sudo apt-get install libopenal-dev
- sudo apt-get install libsndfile1-dev

# build SFML
- cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -D BUILD_SHARED_LIBS=TRUE .
- make
- sudo make install

# install box2d
- wget https://github.com/erincatto/Box2D/archive/master.zip
- unzip master.zip -d box2d-sources
- sudo apt-get install libglew-dev
- sudo apt-get install xorg-dev

export LD_LIBRARY_PATH=/home/morda/dev/gravity/sfml-sources/SFML-2.3/lib:/home/morda/dev/gravity/box2d-sources/Box2D-master/Box2D/Box2D

# compile programm and run
- sh compile.sh
 ./sfml-app
