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
}

void writethrough::PrWr(ulong addr, int processor_number) {
}

void writethrough::BusRd(ulong addr) {
}

void writethrough::BusWr(ulong addr) {
}

bool writethrough::writeback_needed(cache_state state) {
	return true;
}
