

ipcdump: ipcdump.c
	gcc -o $@ $^  -Wall


install: ipcdump
	install -m 0755 ipcdump ./ipcdump

.PHONY: clean


clean:
	rm ipcdump

