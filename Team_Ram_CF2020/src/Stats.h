/*
 * Stats.h
 *
 *  Created on: Jan 10, 2021
 *      Author: mohab
 */

#ifndef STATS_H_
#define STATS_H_

class Stats {
    unsigned int useful = 0;
    unsigned int retrans = 0;
    unsigned int dropped = 0;
public:
    Stats() {};
    void addUseful() { useful++;}
    void addRetrans() { retrans++;}
    void addDropped() { dropped++; useful--;}
    unsigned int getUseful() { return useful;}
    unsigned int getTotal() { return useful + retrans;}
    unsigned int getDropped() { return dropped;}
    unsigned int getRetrans() { return retrans;}
    float getEfficiency() { return ((float)useful)/getTotal(); }
};

#endif /* STATS_H_ */
