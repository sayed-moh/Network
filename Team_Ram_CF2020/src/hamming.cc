/*
 * hamming.cc
 *
 *  Created on: Jan 5, 2021
 *      Author: Mazen Ayman
 */

#include "hamming.h"


long binaryToDecimal(long n)
{
    long num = n;
    long dec_value = 0;
    long base = 1;
    long temp = num;
    while (temp)
    {
        long last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    return dec_value;
}

vector<bool > StringToVectorOfBool(string data)
{
    vector<bool > data_vect;
    vector<bitset<8> > bits;
    string data_str = "";
    for(auto it = data.begin(); it != data.end() ; it++)
    {
        bits.push_back(*it);
    }
    for(auto it = bits.begin(); it != bits.end() ; it++)
    {
        data_str = data_str + it->to_string();
    }
    for(auto it = data_str.begin(); it != data_str.end() ; it++)
    {
        if (*it == '1')
        {
            data_vect.push_back(1);
        }
        else
        {
            data_vect.push_back(0);
        }
    }
    return data_vect;
}

string VectorOfBoolToString(vector<bool > &data)
{
    string current_str = "";
    string data_str = "";
    unsigned int i = 0;
    for (i = 0; i < data.size(); i++)
    {
        if (i % 8 == 0)
        {
            if (i != 0)
            {
                data_str = data_str + char(binaryToDecimal(stol(current_str)));
            }
            current_str = "";
        }
        current_str = current_str + to_string(data[i]);
    }
    data_str = data_str + char(binaryToDecimal(stol(current_str)));
    return data_str;
}

int CalculateNoOfParityBits(int m)
{
    int r = 0;
    while(double(m + r + 1) > pow(2, r))
    {
        r = r + 1;
    }
    return r;
}

int CalculatePayloadLength(int m, int r)
{
    int payload_length = m + r;
    return payload_length;
}

vector<bool > GeneratePayload(vector<bool > &data, vector<bool > &parity_bits, int payload_length, int m, int r)
{
    vector<bool > payload(payload_length);
    vector<bool > isPayloadPositionEmpty(payload_length, true);
    int i = 0; int j = 0;
    for (i = 0 ; i < r ; i++)
    {
        payload[int(pow(2, i))-1] = parity_bits[i];
        isPayloadPositionEmpty[int(pow(2, i))-1] = false;
    }
    for (i = 0 ; i < m ; i++)
    {
        for (j = 0 ; j < payload_length ; j++)
        {
            if (isPayloadPositionEmpty[j] == true)
            {
                payload[j] = data[i];
                isPayloadPositionEmpty[j] = false;
                break;
            }
        }
    }
    return payload;
}

void FixParityBits(vector<bool > &parity_bits, vector<bool > &payload, int r, int payload_length)
{
    int i = 0; int j = 0; int k = 0;
    int count = 0;
    for (i = 0 ; i < r ; i++)
    {
        for (j = int(pow(2, i)) - 1 ; j < payload_length ; j = j + (int(pow(2, i)) * 2))
        {
            count = int(pow(2, i));
            k = j;
            while(count > 0 && k < payload_length)
            {
                parity_bits[i] = parity_bits[i] ^ payload[k];
                k = k + 1;
                count = count - 1;
            }
        }
    }
}

void FixPayload(vector<bool > &payload, vector<bool > &parity_bits, int r)
{
    int i = 0;
    for (i = 0 ; i < r ; i++)
    {
        payload[int(pow(2, i))-1] = parity_bits[i];
    }
}

vector<bool > CalculateParityBits(vector<bool > &payload, int r, int payload_length)
{
    int i = 0; int j = 0; int k = 0;
    int count = 0;
    vector<bool > parity_bits(r, 0);
    for (i = 0 ; i < r ; i++)
    {
        for (j = int(pow(2, i)) - 1 ; j < payload_length ; j = j + (int(pow(2, i)) * 2))
        {
            count = int(pow(2, i));
            k = j;
            while(count > 0 && k < payload_length)
            {
                parity_bits[i] = parity_bits[i] ^ payload[k];
                k = k + 1;
                count = count - 1;
            }
        }
    }
    return parity_bits;
}

void CheckAndFixPayload(vector<bool > &received_payload, int r, int payload_length)
{
    vector<bool > received_parity_bits;
    bool noError = true;
    string error_position = "";
    long error_position_long = 0;
    int i = 0;
    for (i = 0; i < r; i++)
    {
        received_parity_bits.push_back(received_payload[int(pow(2, i)) - 1]);
    }
    vector<bool > parity_bits = CalculateParityBits(received_payload, r, payload_length);
    for (i = 0; i < r; i++)
    {
        if (parity_bits[i] != 0)
        {
            noError = false;
        }
    }
    if (noError == false)
    {
        for (i = r-1; i >= 0; i--)
        {
            if (parity_bits[i] == 0)
            {
                error_position = error_position + "0";
            }
            else
            {
                error_position = error_position + "1";
            }
        }
        error_position_long = binaryToDecimal(stol(error_position));
        if (received_payload[error_position_long-1] == 0)
        {
            received_payload[error_position_long-1] = 1;
        }
        else
        {
            received_payload[error_position_long-1] = 0;
        }
    }
}

vector<bool > ExtractData(vector<bool > &payload, int payload_length, int r)
{
    vector<bool > data;
    bool isParityPosition = false;
    int i = 0; int j = 0;
    for (i = 0; i < payload_length; i++)
    {
        isParityPosition = false;
        for (j = 0; j < r; j++)
        {
            if (i == pow(2, j) - 1)
            {
                isParityPosition = true;
                break;
            }
        }
        if (isParityPosition == false)
        {
            data.push_back(payload[i]);
        }
    }
    return data;
}

vector<bool > HammingCode(vector<bool > &data, int m, NodeRole node_type, vector<bool > &received_payload)
{
    int r = CalculateNoOfParityBits(m);
    int payload_length = CalculatePayloadLength(m, r);
    if (node_type==SENDER)
    {
        vector<bool > parity_bits(r, 0);
        vector<bool > payload = GeneratePayload(data, parity_bits, payload_length, m, r);
        FixParityBits(parity_bits, payload, r, payload_length);
        FixPayload(payload, parity_bits, r);
        return payload;
    }
    else
    {
        CheckAndFixPayload(received_payload, r, payload_length);
        vector<bool > data = ExtractData(received_payload, payload_length, r);
        return data;
    }
}
