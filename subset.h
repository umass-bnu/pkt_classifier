/*
 * subset.h
 *
 *  Created on: Oct 7, 2015
 *      Author: Hao
 */

#ifndef SUBSET_H_
#define SUBSET_H_

#include<map>
#include<iostream>
#include<string>
#include<set>
using namespace std;

class subset{
public: map<string, int> table;
	    unsigned nl1;
	    unsigned nl2;

 bool checkin(string str, unsigned field);
      subset();

};

#endif /* SUBSET_H_ */
