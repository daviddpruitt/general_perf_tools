#include <sys/types.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <papi.h>
#include <errno.h>
#include <unistd.h>

struct net_event {
  char *deviceName;
  char *deviceRxName;
  char *deviceTxName;
  int rxCode;
  int txCode;
};
  
size_t getDeviceNames(char ***listNics);
void getEventCodes(char **listOfNics, size_t numDevices,  struct net_event ***eventList);
int initCounters(struct net_event **eventList, size_t numDevices);

int main(int argc, char *argv[])
{
  struct net_event **events;
  char **listOfNics;
  char **eventNames;
  size_t maxDevices = 0, numDevices = 0, currDevice = 0;
  size_t i;
  int noMatch, retval, eventSet;
  long long *values;

  numDevices = getDeviceNames(&listOfNics);

  for (currDevice = 0; currDevice < numDevices; ++currDevice) {
    printf("Device %s \n",listOfNics[currDevice]);fflush(stdout);
  }

  // get event codes (hopefully)
  getEventCodes(listOfNics, numDevices, &events);
 
  printf("event pop complete\n");fflush(stdout);
  for (currDevice = 0; currDevice < numDevices; ++currDevice) {
    printf("Device %u %s txCode: %d rxCode %d\n", currDevice, events[currDevice]->deviceName,
	   events[currDevice]->txCode, events[currDevice]->rxCode);
  }

  printf("Device list complete\n");fflush(stdout);

  eventSet = initCounters(events, numDevices);
  values = (long long*)malloc(sizeof(long long) * numDevices);
  // run test harness
  if (PAPI_reset(eventSet) != PAPI_OK) {
    printf("Error resetting events");
    exit(-1);
  }
  if (PAPI_start(eventSet) != PAPI_OK) {
    printf("Error starting events\n");
    exit(-1);
  }

  // sleep for 5 seconds
  for (i = 0; i < 5; ++i) {
    printf("%d ", (5 - i));fflush(stdout);
    sleep(1);
  }
  printf("\n");

  if (PAPI_stop(eventSet, values) != PAPI_OK) {
    printf("Error stopping/reading events\n");
    exit(-1);
  }

  // print values
  for (i = 0; i < numDevices; ++i) {
    if (values[(2*i)] > (1024 * 1024))
      printf("Event %s\t%7.3f\tMb\n",events[i]->deviceTxName, values[(2 * i)]/(1024.0/1024.0));
    else if (values[(2*i)] > 1024)
      printf("Event %s\t%7.3f\tKb\n",events[i]->deviceTxName, values[(2 * i)]/1024.0);
    else
      printf("Event %s\t%7.3lld\tb\n",events[i]->deviceTxName, values[(2 * i)]);
    if (values[(2*i)+1] > (1024 * 1024))
      printf("Event %s\t%7.3f\tMb\n",events[i]->deviceRxName, values[(2 * i) + 1]/(1024.0*1024.0));
    else if (values[(2*i)+1] > 1024)
      printf("Event %s\t%7.3f\tKb\n",events[i]->deviceRxName, values[(2 * i) + 1]/1024.0);
    else
      printf("Event %s\t%7.3lld\tb\n",events[i]->deviceRxName, values[(2 * i) + 1]);
  }
  
  printf("Complete\n");
  return 0;
}

size_t getDeviceNames(char ***listNics)
{
  struct ifaddrs *current, *previous;
  struct ifaddrs *head;
  size_t maxDevices = 0, currDevice = 0, numDevices = 0;
  int i, noMatch;
  char **listOfNics;
  printf("inn addr %p\n",listOfNics);

  if (getifaddrs(&head)) {
    printf("Error getting nics %s\n",strerror(errno));
    exit(-1);
  }
  
  current = head;
  while (current) {
    maxDevices++;
    current = current->ifa_next;
  } 
  listOfNics = (char **)malloc(sizeof(char *) * maxDevices);
  printf("Alloc'd add %p\n",listOfNics);
  //create a list of device names that doesn't contain any duplicates
  current = head;
  while (current) {
    noMatch = 1;
    i = 0;
    // check to see if the device name is already in our list
    while (noMatch && (i < currDevice)) {
      if (!strcmp(current->ifa_name, listOfNics[i])) {
  	noMatch = 0;
      }
      ++i;
    }
  
    if (noMatch) {
      // go ahead and add it to the list
      size_t length = strlen(current->ifa_name);
      listOfNics[currDevice] = (char *)malloc(sizeof(char) * (length + 1));
      // copy since current gets freed later
      strncpy(listOfNics[currDevice], current->ifa_name, length + 1);
      ++currDevice;
      ++numDevices;
    }
    current = current->ifa_next;
  }
  
  *listNics = listOfNics;
  printf("list out %p", listNics);
  freeifaddrs(head);
  return numDevices;
}

void getEventCodes(char **listOfNics, size_t numDevices, struct net_event ***eventList)
{
  size_t currDevice;
  struct net_event **events;
  int retval;

  // papi initialization
  retval = PAPI_library_init( PAPI_VER_CURRENT );
  if ( retval != PAPI_VER_CURRENT ) {
    printf("PAPI_library_init failed\n",retval);
    exit(1);
  }

  printf("papi init complete\n");fflush(stdout);
  // populate event names
  events = (struct net_event **)malloc(sizeof(struct net_event*) * numDevices);
  char buffer[128];
  
  for (currDevice = 0; currDevice < numDevices; ++currDevice) {
    size_t length = strlen(listOfNics[currDevice]);
    events[currDevice] = (struct net_event *)malloc(sizeof(struct net_event));
    events[currDevice]->deviceName = (char *)malloc(sizeof(char) * (length + 1));
    strncpy(events[currDevice]->deviceName, listOfNics[currDevice], length + 1);
    events[currDevice]->deviceTxName = (char *)malloc(sizeof(char) * (length + 10)); // length + length(":tx:bytes\0")
    strncpy(events[currDevice]->deviceTxName, listOfNics[currDevice], length + 1);
    strcat(events[currDevice]->deviceTxName, ":tx:bytes");
    events[currDevice]->deviceRxName = (char *)malloc(sizeof(char) * (length + 10)); // length + length(":rx:bytes\0")
    strncpy(events[currDevice]->deviceRxName, listOfNics[currDevice], length + 1);
    strcat(events[currDevice]->deviceRxName, ":rx:bytes");
    
    // get device codes (maybe)
    retval = PAPI_event_name_to_code(events[currDevice]->deviceTxName, &events[currDevice]->txCode);
    if (retval != PAPI_OK) {
      printf("Failed to obtain code for %s reason %s", events[currDevice]->deviceTxName, 
	     (retval == PAPI_ENOTPRESET ? "Invalid preset" : "Not Available"));
    }
    retval = PAPI_event_name_to_code(events[currDevice]->deviceRxName, &events[currDevice]->rxCode);
    if (retval != PAPI_OK) {
      printf("Failed to obtain code for %s reason %d", events[currDevice]->deviceRxName, 
	     (retval == PAPI_ENOTPRESET ? "Invalid preset" : "Not Available"));
    }
  }
  *eventList = events;
}

int initCounters(struct net_event **eventList, size_t numDevices)
{
  int eventSet, retVal;
  size_t currDevice;
  
  eventSet = PAPI_NULL;

  // create event set
  if (PAPI_create_eventset(&eventSet) != PAPI_OK){
    return -1;
  }
  // add events to the event set
  for (currDevice = 0; currDevice < numDevices; ++currDevice) {
    if (PAPI_add_event(eventSet, eventList[currDevice]->txCode) != PAPI_OK) {
      return -1;
    }
    if (PAPI_add_event(eventSet, eventList[currDevice]->rxCode) != PAPI_OK) {
      return -1;
    }
  }

  return eventSet;

}
