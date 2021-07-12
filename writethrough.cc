/************************************************************
                        Course          :       CSC456
                        Source          :       msi.cc
                        Instructor      :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
*************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;
#include "main.h"
#include "writethrough.h"

void writethrough::PrRd(ulong addr, int processor_number) {
        cache_state state;
        current_cycle++;
        reads++;
        cache_line * line = find_line(addr);
        if(line == NULL) {
                read_misses++;
                cache_line *newline = allocate_line(addr);
                memory_transactions++;
                newline->set_state(V);
                bus_reads++;
                sendBusRd(addr, processor_number);
        }
        else {
                state=line->get_state();
                if (state == I){
                        read_misses++;
                        cache_line *newline = allocate_line(addr);
                        memory_transactions++;
                        newline->set_state(V);
                        bus_reads++;
                        sendBusRd(addr, processor_number);
                }
             else if (state == V){
                update_LRU(line);
                }
             }
}

void writethrough::PrWr(ulong addr, int processor_number) {
        cache_state state;
    current_cycle++;
    writes++;
    cache_line * line = find_line(addr);
    if (line == NULL || line->get_state() == I){
                write_misses++;
                cache_line *newline = allocate_line(addr);
                memory_transactions++;
                bus_writes++;
                sendBusWrite(addr, processor_number);
     }
    else
{
                update_LRU(line);
                bus_writes++;
                sendBusWrite(addr, processor_number);
                
         }
}

void writethrough::BusRd(ulong addr) {
       memory_transactions++;
       write_backs++;
}

void writethrough::BusWr(ulong addr) {
        cache_line * line=find_line(addr);
        if(line != NULL) {
                if (line->get_state() == V) {
                        invalidations++;
                        line->set_state(I);
                }
        } 
}

bool writethrough::writeback_needed(cache_state state) {
	return true;
}
