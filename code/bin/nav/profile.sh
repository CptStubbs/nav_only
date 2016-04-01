#!/bin/sh

perf record --call-graph dwarf -- ./navexe
perf report -g graph 
