#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "read_netstats.h"

extern int num_devices;
#define NUM_CHECKS

int
main (int argc, char *argv[])
{
	struct net_event **device_list;
	int i, latency, retval;
	
	if (init(&device_list))
		return 1;
	
	printf("Number of devices %d\n", num_devices);
	for (i = 0; i < num_devices; ++i) {
		printf("Device: %s Length of name: %d \n\n", device_list[i]->device_name, device_list[i]->name_length);
	}
	
	latency = compute_latency(&device_list);
	printf("Call latency %d micro-seconds\n", latency);
	
	retval = read_stats(&device_list);
	if (retval) 
		printf("Unable to openfile: %s\n", strerror(errno));

	for (i = 0; i < num_devices; ++i) {
		printf("Device: %s RX bytes: %"PRIu64" TX bytes: %"PRIu64"\n", 
			device_list[i]->device_name, 
			device_list[i]->rx_bytes,
			device_list[i]->tx_bytes);
	}
	// sleep for 5 seconds
	for (i = 0; i < 5; ++i) {
		printf("%d ", (5 - i));fflush(stdout);
		sleep(1);
	}
	printf("\n"); 
	retval = read_stats(&device_list);
	if (retval) 
		printf("Unable to openfile: %s\n", strerror(errno));
		
	for (i = 0; i < num_devices; ++i) {
		printf("\n");
		printf("Device: %s RX bytes: %"PRIu64" TX bytes: %"PRIu64"\nRX throughput %"PRIu64" bytes/us"
				" TX throughput %"PRIu64" bytes/us\nTime Read:%ld.%ld seconds\n", 
			device_list[i]->device_name, 
			device_list[i]->rx_bytes,
			device_list[i]->tx_bytes,
			device_list[i]->rx_throughput,
			device_list[i]->tx_throughput,
			device_list[i]->last_read.tv_sec,
			device_list[i]->last_read.tv_nsec);

	}
	
	
	return 0;
}
