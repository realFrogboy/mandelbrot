CXX = g++

mandelbrot1: mandelbrot1.cpp
	${CXX} -c -g -mavx -mavx2 $<
	${CXX} mandelbrot1.o -o $@ -lsfml-graphics -lsfml-window -lsfml-system

mandelbrot2: mandelbrot2.cpp
	${CXX} -c -g -mavx -mavx2 $<
	${CXX} mandelbrot2.o -o $@ -lsfml-graphics -lsfml-window -lsfml-system

overlay: overlay.cpp
	${CXX} -c -g -mavx -mavx2 $<
	${CXX} overlay.o -o $@ -lsfml-graphics -lsfml-window -lsfml-system
