all: main image convert

main: main.cc
	g++ main.cc -std=c++17 -o ./output/main

image: main
	./output/main > ./output/image.ppm

convert: ./output/image.ppm
	convert ./output/image.ppm ./output/image.png

clean:
	rm -rf image.ppm
