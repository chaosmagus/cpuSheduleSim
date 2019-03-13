TOBY CROCKER
CSCI442
project01.2

INSTRUCTIONS TO RUN:
        type make to compile, enter the following to run:
        ./simulation [flags] <filename>

        VALID FLAGS:
        -t, --per_thread
            Output additional per-thread statistics for arrival time, service
            time, etc.

        -v, --verbose
            Output information about every state-changing event and scheduling
            decision.

        -h, --help
            Display this README file.

        -a, --algorithm
            The scheduling algorithm to use, requires an argument from the following:
            FCFS, RR, PRIORITY, or CUSTOM.

PROGRAM DETAILS:
    simulation.cpp creates a simulator object which
      - reads input data
      - creates and stores necessary data structures
      - adds event to "ARRIVAL" priority queue
      - runs the selected algorithm
    
    Simulator.cpp/.hpp
        - Simulator class holds scheduling algorithm implementations, and the high level containers

    Proc.cpp/.hpp
        

    Thread.cpp/.hpp
        - Thread class tracks parent proc, child bursts, priority, state, and the data 
          needed for tracking metrics.

    Burst.cpp/.hpp

    Event.cpp/.hpp


time elapsed - about 15 hours so far

