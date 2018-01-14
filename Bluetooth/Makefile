CC = gcc
CFLAGS = -Wall -Werror

main: Bluetooth

Bluetooth:
	$(CC) node.c $(CFLAGS) -lbluetooth -o node
	$(CC) master.c $(CFLAGS) -lbluetooth -lpthread -o master

clean:
	rm node
	rm master
