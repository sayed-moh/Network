/*
 * hamming.h
 *
 *  Created on: Jan 5, 2021
 *      Author: Mazen Ayman
 */

#ifndef HAMMING_H_
#define HAMMING_H_

#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <math.h>

using namespace std;


enum NodeRole{SENDER, RECEIVER};



//CONVERT STRING TO VECTOR OF BOOL
vector<bool > StringToVectorOfBool(string data);
//CONVERT VECTOR OF BOOL TO STRING
string VectorOfBoolToString(vector<bool > &data);
//CALCULATING THE VALUE OF "r"
int CalculateNoOfParityBits(int m);
//CALCULATING THE TOTAL LENGTH OF THE PAYLOAD AFTER ADDING "r" TO THE ORIGINAL LENGTH
int CalculatePayloadLength(int m, int r);
//GENERATE PAYLOAD (USED BY SENDER)
vector<bool > GeneratePayload(vector<bool > &data, vector<bool > &parity_bits, int payload_length, int m, int r);
//CALCULATE THE CORRECT VALUES OF THE PARITY BITS (USED BY SENDER)
void FixParityBits(vector<bool > &parity_bits, vector<bool > &payload, int r, int payload_length);
//WRITE THE CORRECT VALUES OF THE PARITY BITS IN THE PAYLOAD (USED BY SENDER)
void FixPayload(vector<bool > &payload, vector<bool > &parity_bits, int r);
//SUBSTITUTING THE PARITY BITS CALCULATED BY THE SENDER (USED BY RECEIVER)
vector<bool > CalculateParityBits(vector<bool > &payload, int r, int payload_length);
//CONVERT BINARY TO DECIMAL
long binaryToDecimal(long n);
//CHECK IF THERE IS AN ERROR OCCURED DURING THE TRANSMISSION AND CORRECT IT (USED BY THE RECEIVER)
void CheckAndFixPayload(vector<bool > &received_payload, int r, int payload_length);
//EXTRACT DATA FROM PAYLOAD
vector<bool > ExtractData(vector<bool > &payload, int payload_length, int r);
//APPLYING HAMMING CODE (SENDER OR RECEIVER)
vector<bool > HammingCode(vector<bool > &data, int m, NodeRole node_type, vector<bool > &received_payload);


#endif /* HAMMING_H_ */
