/************************************************************
                        Course          :       CSC506
                        Source          :       mosi.cc
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
#include "mosi.h"

void MOSI::PrRd(ulong addr, int processor_number) {
        cache_state state;
        reads++;
        current_cycle++;
        cache_line *line = find_line(addr);
        if (line == NULL) {
             read_misses++;
             cache_line *newline = allocate_line(addr);
             memory_transactions++;
                I2S++;
             newline->set_state(S);
             bus_reads++;
             sendBusRd(addr, processor_number);
       }
        else {
                state=line->get_state();

                if (state == I){
                        read_misses++;
                        cache_line *newline = allocate_line(addr);
                        memory_transactions++;
                        I2S++;
                        newline->set_state(S);
                        bus_reads++;
                        sendBusRd(addr, processor_number);
                }
             else if (state == M || state == S || state == O){
                memory_transactions++;
                update_LRU(line);
                }
        }
}


void MOSI::PrWr(ulong addr, int processor_number) {
        cache_state state;
        current_cycle++;
        writes++;
        cache_line * line = find_line(addr);
        if (line == NULL || line->get_state() == I) {
                write_misses++;
                cache_line *newline = allocate_line(addr);
                memory_transactions++;
                I2M++;
                newline->set_state(M);
                bus_readxs++;
                sendBusRdX(addr, processor_number);
        }
        else {
                state=line->get_state();
                if (state == M) {
                        memory_transactions++;
                        update_LRU(line);
                }
                else if (state == S) {
                        S2M++;
                        line->set_state(M);
                        update_LRU(line);
                        bus_upgrades++;
                        sendBusUpgr(addr, processor_number);
                }
                else if (state == O) {
                        O2M++;
                        line->set_state(M);
                        update_LRU(line);
                        bus_upgrades++;
                        memory_transactions++;
                        sendBusUpgr(addr, processor_number);
                }
         }
}


void MOSI::BusRd(ulong addr) {
        cache_state state;
        cache_line * line=find_line(addr);
        if (line!=NULL) {
                state = line->get_state();
                if (state == M) {
                      flushes++;
                      line->set_state(O);
                      M2O++;
                }
                else if (state == O) {
                      flushes++;
                }
                else if (state == S){
                        cache2cache++;
                }
        }
}

void MOSI::BusRdX(ulong addr) {
        cache_line * line=find_line(addr);
        if (line!=NULL) {
                cache_state state;
                state=line->get_state();
                if (state == S ) {
                        invalidations++;
                        line->set_state(I);
                }
                else if (state == M )
                {
                        flushes++;
                        invalidations++;
                        line->set_state(I);
                }
                else if (state == O)
                {
                        flushes++;
                        invalidations++;
                        line->set_state(I);
                }
                else if (state == I) {
                        //nothing happens
                }       
        }
}


void MOSI::BusUpgr(ulong addr) {
    cache_state state;
    cache_line * line=find_line(addr);
    if (line!=NULL)
       {
                state = line->get_state();
                if (state == O) {
                      invalidations++;
                      line->set_state(I);
                }
                else if (state == S) {
                      invalidations++;
                      line->set_state(I);
                }
                else if (state == I) {
                      // nothing happens
                }
        }
}


bool MOSI::writeback_needed(cache_state state) {
    if(state == M) {
        return true;
    }
    return false;
}
