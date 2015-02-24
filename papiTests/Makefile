all: hwctrs net_test netlink

hwctrs: hwctrs.c handle_error.c
	gcc handle_error.c hwctrs.c -lpapi -o hwctrs

net_test: net_test.c handle_error.c
	gcc handle_error.c net_test.c -lpapi -o net_test
	
netlink: rtnetlink_test.c read_netstats.c
	gcc -O1 rtnetlink_test.c read_netstats.c -o netlink
	
handle_error.o:
	gcc -I. -c handle_error.c

clean:
	rm -rf hwctrs net_test netlink
