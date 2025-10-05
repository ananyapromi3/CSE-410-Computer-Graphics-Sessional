# g++ main_2D.cpp -o main_2D -framework OpenGL -framework GLUT
g++ main.cpp vector.cpp point.cpp ball.cpp plane.cpp -o main -lGL -lGLU -lglut
./main