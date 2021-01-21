

ipcdump: ipcdump.c
	gcc -o $@ $^  -Wall


install: ipcdump
	install -m 0755 /usr/bin/ipcdump ./ipcdump

.PHONY: clean


clean:
	rm ipcdump

