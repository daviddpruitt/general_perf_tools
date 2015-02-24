#ifndef _NET_STATS_H
#define _NET_STATS_H

#include <inttypes.h>
#include <time.h>
#define NET_BUFFER_SIZE
#define NET_STATS_FILE "/proc/net/dev"

/* the number of network devices */
int num_devices;

/* if the system has been inited */
extern int netstats_init_complete;

struct net_event {
  char device_name[16];   /* these are usually short, avoid malloc */
  int name_length;	  /* length of the device name, saved so we only have to compute once */
  uint64_t rx_bytes;
  uint64_t tx_bytes;
  uint64_t rx_bytes_last;
  uint64_t tx_bytes_last;
  uint64_t rx_throughput;
  uint64_t tx_throughput;
  struct timespec last_read;
};

/* read net stats straight into a buffer
 * this reads the stats from the file directly
 */
int read_raw(char buffer[], size_t length);

/* 
 * read the netstats into a set of structs 
 */
int read_stats(struct net_event ***devices);

/* 
 * initialize the interface 
 */
int init(struct net_event ***devices);

/*
 * computes the latency of the read_stats call
 * the values will be those at the time the
 * test is concluded
 */
int compute_latency(struct net_event ***devices);

#endif // _NET_STATS_H
