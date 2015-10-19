/*
 * tree.h
 *
 *  Created on: Sep 23, 2015
 *      Author: Hao
 */

#ifndef TREE_H_
#define TREE_H_
#include <math.h>
#include<iostream>
#include<vector>
#include<string>
#include<set>
#include "subset.h"

using namespace std;

//range structure denote address or protocol range
struct range {
		unsigned low;    //lower bound
		unsigned high;   //higher bound
	};

//rule structure in each field
struct pc_rule {
		int ruleid;
		range field[5];
	};

struct node{
		int black;
		bool *nlt;   //bit-map indicate rule belongs to which subset
		unsigned np;        //denote nested/prefix level relation
		node *lc;
	    node *rc;
	};


class tree{
	    int num_node;          //denote the number of nodes in tree
public: node *root;     //denote the root of the tree

public:	tree(int n = 0);
		~tree(){delete root; };
		void insert( range ip_add);
		void nl_pl( range ip_add);
		int level(range ip_add);
		string lookup( unsigned add);   //LPM
		string lookup1(range ip_add);
		node *lpm( unsigned add);   //LPM return node pointer
		node *lpm1(range ip_add);
		int initial_p1(int numrules, range *fieldrule);
		void initial_p2(subset *sset,range *fieldrule, int numrules, int count, unsigned field);
		int treesize();
};



#endif /* TREE_H_ */
