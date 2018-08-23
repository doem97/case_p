# Introduction
This project is the simulation of the CASE+ structure. CASE+ is a improved version of CASE structure, in which CASE stands for *Cache-Assistant Stretchable Estimator* in INFOCOM paper:

>Li, Yang, et al. "Case: Cache-assisted stretchable estimator for high speed per-flow measurement." INFOCOM 2016-The 35th Annual IEEE International Conference on Computer Communications, IEEE. IEEE, 2016.

This project simulates CASE+ structure in C++. It reads real network trace from CAIDA and process them through CASE+ structure, and finally get some stastic results and timing information. It can mainly test:

- Throughput
- Simulation of different threshold and different on-chip cache size
- Select whether use packet number or byte value as the threshold that determines when a flow will be sent to LRU_2 from LRU_1
- On-off chip interaction frequency
- Relative error in saving flow information