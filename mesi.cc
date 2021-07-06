/************************************************************
                        Course          :       CSC506
                        Source          :       mesi.cc
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
#include "mesi.h"
#include "cache.h"

void MESI::PrRd(ulong addr, int processor_number) {
	
	cache_state state;
	current_cycle++;
	reads++; 
	cache_line* line = find_line(addr);

	if ((line == NULL) || (line->get_state() == I)) {
        cache_line *newline = allocate_line(addr);
        read_misses++;
	    if (sharers_exclude(addr, processor_number) > 0) {
            cache2cache++;
            newline->set_state(S);
        }
        else {
            memory_transactions++;
            newline->set_state(E);
            }
	    
	    bus_reads++;
	    sendBusRd(addr, processor_number);
    }
    else {
		state = line->get_state();
        if (state == E || state == M || state == S) {
            update_LRU(line);
        }
	}
}

void MESI::PrRdDir(ulong addr, int processor_number) {
}

void MESI::PrWrDir(ulong addr, int processor_number) {
}

void MESI::PrWr(ulong addr, int processor_number){

    cache_state state;
    current_cycle++;
    writes++; 
    cache_line * line = find_line(addr);

    if ((line == NULL) || (line->get_state() ==I)){
        write_misses++;
        cache_line *newline = allocate_line(addr);
        if (sharers_exclude(addr, processor_number) > 0) {
            cache2cache++;
            }
            else {
            memory_transactions++;
            }
            newline->set_state(M);
            bus_readxs++;
	        sendBusRdX(addr, processor_number);
    }

    else {
        state = line->get_state();
        if (state == E){
            line->set_state(M);
            update_LRU(line);
         }

        else if (state == M){
            update_LRU(line);
         }

        else if (state == S){
            line->set_state(M);
            update_LRU(line);
            bus_upgrades++; 
            sendBusUpgr(addr, processor_number);
         }
     }
}

void MESI::BusRd(ulong addr) {
    cache_state state;
    cache_line * line = find_line(addr);

    if(line !=NULL){
        state = line -> get_state();

        if (state == M){
            interventions++; 
            flushes++; 
            write_backs++;
            memory_transactions++;
            line -> set_state(S);
        }

         else if(state == S){
         }

        else if (state == E){
            interventions++; 
            line -> set_state(S);
        }
    }
}

void MESI::BusRdX(ulong addr) {
    cache_state state;
    cache_line * line = find_line(addr);

    if (line != NULL){
        state = line -> get_state();
            
            if (state == S){
                invalidations++; 
                line->set_state(I);
            }

            else if (state == M){
                flushes++; 
                write_backs++; 
                memory_transactions++;
                invalidations++; 
                line->set_state(I);
            }

            else if (state == E){
                invalidations++; 
                line->set_state(I);
            }
    }
}

void MESI::BusUpgr(ulong addr) {
    cache_state state;
    cache_line *line = find_line(addr);

    if(line!=NULL) {
      state = line->get_state();

      if(state == S){
        line->set_state(I);
        invalidations++; 
       }
    }
}


bool MESI::writeback_needed(cache_state state) {
    if (state == M){
        return true;
    } 
    else {
        return false;
    }
}

void MESI::signalRd(ulong addr, int processor_number) {
}


void MESI::signalRdX(ulong addr, int processor_number){	
}

void MESI::signalUpgr(ulong addr, int processor_number){
}

void MESI::Int(ulong addr) {
}


void MESI::Inv(ulong addr) {
}


cache_line * MESI::allocate_line_dir(ulong addr) {
    return NULL;
}
