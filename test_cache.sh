#!/bin/bash

OUTN=4
swaptions_truncated='/afs/eos.ncsu.edu/lockers/workspace/csc/CSC506-1/trace/swaptions_truncated'
for protocol in 1
do
for cachesize in 128000 256000 512000
do
 echo "./simulate_cache smp $cachesize 1 64 16 $protocol $swaptions_truncated" > real.$OUTN
 ./simulate_cache smp $cachesize 1 64 16 $protocol $swaptions_truncated >> real.$OUTN
 echo "./simulate_cache smp $cachesize 1 64 16 $protocol $swaptions_truncated" > ref.$OUTN
 ./simulate_cache_ref smp $cachesize 1 64 16 $protocol $swaptions_truncated >> ref.$OUTN
 diff real.$OUTN ref.$OUTN > diff.$OUTN
 OUTN=$(($OUTN+1))
done
for associativity in 2 4
do
 echo "./simulate_cache smp 256000 $associativity 64 16 $protocol $swaptions_truncated" > real.$OUTN
 ./simulate_cache smp 256000 $associativity 64 16 $protocol $swaptions_truncated >> real.$OUTN
 echo "./simulate_cache smp 256000 $associativity 64 16 $protocol $swaptions_truncated" > ref.$OUTN
 ./simulate_cache_ref smp 256000 $associativity 64 16 $protocol $swaptions_truncated >> ref.$OUTN
 diff real.$OUTN ref.$OUTN > diff.$OUTN
 OUTN=$(($OUTN+1))
done
for blocksize in 32 128 256
do
 echo "./simulate_cache smp 256000 1 $blocksize 16 $protocol $swaptions_truncated" > real.$OUTN
 ./simulate_cache smp 256000 1 $blocksize 16 $protocol $swaptions_truncated >> real.$OUTN
 echo "./simulate_cache smp 256000 1 $blocksize 16 $protocol $swaptions_truncated" > ref.$OUTN
 ./simulate_cache_ref smp 256000 1 $blocksize 16 $protocol $swaptions_truncated >> ref.$OUTN
 diff real.$OUTN ref.$OUTN > diff.$OUTN
 OUTN=$(($OUTN+1))
done
done
