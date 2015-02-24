#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "read_netstats.h"

struct net_event **net_devices;
FILE *net_stats_file;

/* 
 * initialize the interface 
 */
int init(struct net_event ***devices)
{
  char buffer[256];
  char extra[256];
  char single[1];
  uint64_t unused[16];
  struct net_event **dev_list;
  int i;
  
  // determine number of devices
  num_devices = 0;

  net_stats_file = fopen(NET_STATS_FILE, "rt");
  if(!net_stats_file) {
    fprintf(stderr, "Unable to openfile: %s\n", strerror(errno));
    return errno;
  }
  
  // first two lines are headers
  fgets(buffer, 256, net_stats_file);
  fgets(buffer, 256, net_stats_file);

  // cycle through file to see how many devices there are
  while (fgets(buffer, 256, net_stats_file) != NULL) {
    ++num_devices;
    if (num_devices > 10)
      break;
  }
  
  fclose(net_stats_file);

  net_stats_file = fopen(NET_STATS_FILE, "rt");
  if(!net_stats_file) {
    fprintf(stderr, "Unable to openfile: %s\n", strerror(errno));
    return errno;
  }

  // first two lines are headers
  fgets(buffer, 256, net_stats_file);
  fgets(buffer, 256, net_stats_file);

  // now we can setup devices
  dev_list = (struct net_event**)malloc(sizeof(struct net_event*) * num_devices);
  
  for (i = 0; i < num_devices; ++i) {
    dev_list[i] = (struct net_event*)malloc(sizeof(struct net_event));
    fgets(buffer, 256, net_stats_file);
    sscanf(buffer,"%[^:]%n %1s %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %[^'\n'] " , 
           dev_list[i]->device_name, 
           &dev_list[i]->name_length,
           single,
           &dev_list[i]->rx_bytes,
           &unused[0],
           &unused[1],
           &unused[2],
           &unused[3],
           &unused[4],
           &unused[5],
           &unused[6],
           &dev_list[i]->tx_bytes,
           extra);
  }
  
  
  *devices = dev_list;
  netstats_init_complete = 1;
  return 0;
}	

/* read net stats straight into a buffer
 * this reads the stats from the file directly
 */
int read_raw(char buffer[], size_t length)
{
  net_stats_file = fopen(NET_STATS_FILE, "rt");
  if(!net_stats_file) {
    buffer = NULL;
    return 1;
  }
  fscanf(net_stats_file, "%length[^'\0']", buffer);
  return 0;
};

/* 
 * read the netstats into a set of structs 
 */
#ifdef NORMAL
int read_stats(struct net_event ***devices)
{
  struct net_event **dev_list;
  char buffer[256];
  char extra[256];
  char single[1];
  uint64_t unused[16];
  int i;

  dev_list = *devices;
  net_stats_file = fopen(NET_STATS_FILE, "rt");
  if(!net_stats_file) {
    fprintf(stderr, "Unable to openfile: %s\n", strerror(errno));
    return 1;
  }

  // first two lines are headers
  fgets(buffer, 256, net_stats_file);
  fgets(buffer, 256, net_stats_file);

  for (i = 0; i < num_devices; ++i) {
    fgets(buffer, 256, net_stats_file);
    sscanf(buffer,"%[^:]%n %1s %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %[^'\n'] " , 
	   dev_list[i]->device_name, 
	   &dev_list[i]->name_length,
	   single,
	   &dev_list[i]->rx_bytes,
	   &unused[0],
	   &unused[1],
	   &unused[2],
	   &unused[3],
	   &unused[4],
	   &unused[5],
	   &unused[6],
	   &dev_list[i]->tx_bytes,
	   extra);
  }

  fclose(net_stats_file);
  *devices = dev_list;
  return 0;
}
#else
int read_stats(struct net_event ***devices)
{
  struct net_event **dev_list;
  char buffer[256];
  char stream_buff[1536];
  char extra[256];
  char single[1];
  uint64_t unused[16];
  struct timespec curr_time;
  time_t diff;
  int i;
  int net_file_fd;

  dev_list = *devices;
  net_stats_file = fopen(NET_STATS_FILE, "r");

  if(!net_stats_file) {
    //fprintf(stderr, "Unable to openfile: %s\n", strerror(errno));
    return 1;
  }

  setvbuf(net_stats_file, stream_buff, _IOLBF, 1536);

  // first two lines are headers
  fgets(buffer, 256, net_stats_file);
  fgets(buffer, 256, net_stats_file);

  for (i = 0; i < num_devices; ++i) {
    // update last counts
    dev_list[i]->rx_bytes_last = dev_list[i]->rx_bytes;
    dev_list[i]->tx_bytes_last = dev_list[i]->tx_bytes;

    clock_gettime(CLOCK_REALTIME, &curr_time);

    fgets(buffer, 256, net_stats_file);
    sscanf(buffer,"%[^:]%n %1s %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %[^'\n'] " , 
	   dev_list[i]->device_name, 
	   &dev_list[i]->name_length,
	   single,
	   &dev_list[i]->rx_bytes,
	   &unused[0],
	   &unused[1],
	   &unused[2],
	   &unused[3],
	   &unused[4],
	   &unused[5],
	   &unused[6],
	   &dev_list[i]->tx_bytes,
	   extra);

    // calculate throughput, we use realtime since we can't attribute
    // network stats to a particular thread
    diff = (((curr_time.tv_sec - dev_list[i]->last_read.tv_sec) * 1000000000) 
      + (curr_time.tv_nsec - dev_list[i]->last_read.tv_nsec)) / 1000000;
    dev_list[i]->rx_throughput = (diff ? ((dev_list[i]->rx_bytes - dev_list[i]->rx_bytes_last) / diff) : 0); 
    dev_list[i]->tx_throughput = (diff ? ((dev_list[i]->tx_bytes - dev_list[i]->tx_bytes_last) / diff) : 0);
    dev_list[i]->last_read.tv_sec = curr_time.tv_sec;
    dev_list[i]->last_read.tv_nsec = curr_time.tv_nsec;
  }

  fclose(net_stats_file);

  *devices = dev_list;
  return 0;
}
#endif


/*
 * computes the latency of the read_stats call
 * in microseconds
 */
int compute_latency(struct net_event ***devices) 
{
  size_t num_calls = 100000;
  size_t i;
  time_t diff;
  struct timespec begin;
  struct timespec end;

  clock_gettime(CLOCK_REALTIME, &begin);
  for (i = 0; i < num_calls; ++i) {
    read_stats(devices);
  }
  clock_gettime(CLOCK_REALTIME, &end);

  diff = (((end.tv_sec - begin.tv_sec) * 1000000000) + (end.tv_nsec - begin.tv_nsec)) / 1000;
  
  return (diff / (num_calls/1000));
}
int netstats_init_complete = 0;
