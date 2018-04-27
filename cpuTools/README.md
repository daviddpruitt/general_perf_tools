# This directory contains a couple of cpu tools

## setcpulatency

Taken from:
Controlling Processor C-State Usage in Linux
published by Dell
https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&cad=rja&uact=8&ved=0ahUKEwjEwKXE9NXaAhVNMawKHXp2BRMQFghKMAI&url=http%3A%2F%2Fen.community.dell.com%2Fcfs-file%2F__key%2Ftelligent-evolution-components-attachments%2F13-4491-00-00-20-22-77-64%2FControlling_5F00_Processor_5F00_C_2D00_State_5F00_Usage_5F00_in_5F00_Linux_5F00_v1.1_5F00_Nov2013.pdf&usg=AOvVaw26a-L6ovUe9RaW8A9Z7XVg

This sets the minimum latency for CPU wakeups and can be used to dynamically
limit C-State usage, for example restricting to C0 or C1

### Table of Latencies (in us)

Broadwell (i5-5200u) 
C0      0
C1      2
C1e    10
C3     40
C6    133
C7s   166
C8    300
C9    600 
C10  2600

## checkAvailFreqs.py

This tool checks what CPU frequencies can be set via the dev interfaces.
Its useful because with modern drivers the frequeny requested isn't 
always the frequency that is set. This tool sets many different frequencies
and returns unique frequncies.

The results have to be checked to ensure consistency and sanity. Some 
frequencies may not be possible or may have unintended side effects.
