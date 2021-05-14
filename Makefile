default: all

all: clean
	gcc -o calgur -g main.c wallpaper.c helpers.c cJSON.c calgur.c $(shell pkg-config --cflags --libs libcurl)

clean:
	rm -rf calgur

test: all
	./calgur
