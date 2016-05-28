g++ -c main.cpp -I /home/morda/dev/gravity/sources/sfml-sources/SFML-2.3/include -I /home/morda/dev/gravity/sources/box2d-sources/Box2D-master/Box2D/
if [ $? -gt 0 ];then
    echo error main.o generation error
    exit
fi

echo main.o generated ...
g++ main.o -o sfml-app -L/home/morda/dev/gravity/sources/sfml-sources/SFML-2.3/lib -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -L/home/morda/dev/gravity/sources/box2d-sources/Box2D-master/Box2D/Box2D -lBox2D
if [ $? -gt 0 ];then
    echo main.o compilation error
    exit
fi
echo main.o compiled ...
