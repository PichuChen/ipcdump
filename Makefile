

ipcdump: ipcdump.c
	gcc -o $@ $^  -Wall


install: ipcdump
	# On macOS
	install -m 0755 ipcdump /usr/local/bin

.PHONY: clean


clean:
	rm ipcdump

