default: all

all: clean
	gcc -fPIC -shared wallpaper.c cJSON.c -o libwallpaper.so
	gcc -c -g main.c
	gcc -c -g helpers.c
	gcc -c -g cJSON.c
	gcc -c -g calgur.c
	gcc calgur.o main.o helpers.o cJSON.o -L./ -lwallpaper -o calgur $(shell pkg-config --cflags --libs libcurl)

clean:
	rm -rf calgur *.so *.o

test: all
	./calgur
