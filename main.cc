/*
 * main.cc
 *
 *  Created on: Sep 23, 2015
 *      Author: Hao
 */
#include "tree.h"
#include "subset.h"
#include "bloom_filter.hpp"
#include<cstdio>
#include<set>
#include<utility>
#include<map>
#include<bitset>
#include<string>
#include<cstring>
#include<vector>
#define TUPLE 2

using namespace std;

//The function is to load the rules from filter set txt file
//line by line to array named as rule[]
int loadrule(FILE *fp, struct pc_rule *rule) {

	int tmp;
	unsigned sip1, sip2, sip3, sip4, siplen;
	unsigned dip1, dip2, dip3, dip4, diplen;
	unsigned proto, protomask;
	int i = 0;

	while (1) {

		if (fscanf(fp, "@%d.%d.%d.%d/%d %d.%d.%d.%d/%d %d : %d %d : %d %x/%x\n",
				&sip1, &sip2, &sip3, &sip4, &siplen, &dip1, &dip2, &dip3, &dip4,
				&diplen, &rule[i].field[2].low, &rule[i].field[2].high,
				&rule[i].field[3].low, &rule[i].field[3].high, &proto,
				&protomask) != 16)
			break;
		if (siplen == 0) {
			rule[i].field[0].low = 0;
			rule[i].field[0].high = 0xFFFFFFFF;
		} else if (siplen > 0 && siplen <= 8) {
			tmp = sip1 << 24;
			rule[i].field[0].low = tmp;
			rule[i].field[0].high = rule[i].field[0].low + (1 << (32 - siplen))
					- 1;
		} else if (siplen > 8 && siplen <= 16) {
			tmp = sip1 << 24;
			tmp += sip2 << 16;
			rule[i].field[0].low = tmp;
			rule[i].field[0].high = rule[i].field[0].low + (1 << (32 - siplen))
					- 1;
		} else if (siplen > 16 && siplen <= 24) {
			tmp = sip1 << 24;
			tmp += sip2 << 16;
			tmp += sip3 << 8;
			rule[i].field[0].low = tmp;
			rule[i].field[0].high = rule[i].field[0].low + (1 << (32 - siplen))
					- 1;
		} else if (siplen > 24 && siplen <= 32) {
			tmp = sip1 << 24;
			tmp += sip2 << 16;
			tmp += sip3 << 8;
			tmp += sip4;
			rule[i].field[0].low = tmp;
			rule[i].field[0].high = rule[i].field[0].low + (1 << (32 - siplen))
					- 1;
		} else {
			printf("Src IP length exceeds 32\n");
			return 0;
		}

		if (diplen == 0) {
			rule[i].field[1].low = 0;
			rule[i].field[1].high = 0xFFFFFFFF;
		} else if (diplen > 0 && diplen <= 8) {
			tmp = dip1 << 24;
			rule[i].field[1].low = tmp;
			rule[i].field[1].high = rule[i].field[1].low + (1 << (32 - diplen))
					- 1;
		} else if (diplen > 8 && diplen <= 16) {
			tmp = dip1 << 24;
			tmp += dip2 << 16;
			rule[i].field[1].low = tmp;
			rule[i].field[1].high = rule[i].field[1].low + (1 << (32 - diplen))
					- 1;
		} else if (diplen > 16 && diplen <= 24) {
			tmp = dip1 << 24;
			tmp += dip2 << 16;
			tmp += dip3 << 8;
			rule[i].field[1].low = tmp;
			rule[i].field[1].high = rule[i].field[1].low + (1 << (32 - diplen))
					- 1;
		} else if (diplen > 24 && diplen <= 32) {
			tmp = dip1 << 24;
			tmp += dip2 << 16;
			tmp += dip3 << 8;
			tmp += dip4;
			rule[i].field[1].low = tmp;
			rule[i].field[1].high = rule[i].field[1].low + (1 << (32 - diplen))
					- 1;
		} else {
			printf("Dest IP length exceeds 32\n");
			return 0;
		}
		if (protomask == 0xFF) {
			rule[i].field[4].low = proto;
			rule[i].field[4].high = proto;
		} else if (protomask == 0) {
			rule[i].field[4].low = 0;
			rule[i].field[4].high = 0xFF;
		} else {
			printf("Protocol mask error\n");
			return 0;
		}
		rule[i].ruleid = i + 1;
		i++;
	}

	return i;
}

int main(int argc, char *argv[]){

	int i = 0;
	unsigned j = 0;        //loop control
	unsigned k = 0;
	int count = 0;        //count the number of subsets
	int numrules = 0;
	FILE *fp;
	pc_rule *ruleset;
	char *s = new char[200];

	map<string, int> table;      //whole rules are stored in table
	map<string, int>::iterator itm;

	fp = fopen("fw1.txt", "r"); // when used in eclipse environment, choose this line.
	if (fp == NULL)
		printf("can't open ruleset file\n");

	while (fgets(s, 200, fp) != NULL)
		numrules++;
	rewind(fp);
	delete s;

	cout<<"volum of ruleset is "<<numrules<<endl;
	ruleset = new pc_rule[numrules];

	numrules = loadrule(fp, ruleset);
	cout<<"successful load "<<numrules<<" rules."<<endl;

	tree srctree(1);
	tree dsttree(1);
	range srcip[numrules];
	range dstip[numrules];
	for(i = 0; i != numrules; i++){
		srcip[i] = ruleset[i].field[0];
		dstip[i] = ruleset[i].field[1];
	}
	unsigned srclevel, dstlevel;
	srclevel = srctree.initial_p1(numrules, srcip);
	dstlevel = dsttree.initial_p1(numrules, dstip);


//construct the whole ruleset into table(only store unique value)
	for(i = 0; i != numrules; i++){
		string key = srctree.lookup1(ruleset[i].field[0]);
		key = key + ' ' + dsttree.lookup1(ruleset[i].field[1]);
//		cout<<key<<endl;  //test use
		table.insert(make_pair(key,1));
	}
	cout<<"table size = "<<table.size()<<endl;

//partitioning ruleset part
    set<string> field1[srclevel];
    set<string> field2[dstlevel];
    set<string>::iterator its1;
    set<string>::iterator its2;

    for(i = 0; i != numrules; i++){
    	int j, k;
    	j = srctree.level(ruleset[i].field[0]);
    	k = dsttree.level(ruleset[i].field[1]);

    	cout<<"rule number: "<<i<<" level: "<<j<<" "<<k<<endl;
    	field1[j].insert(srctree.lookup1(ruleset[i].field[0]));
    	field2[k].insert(dsttree.lookup1(ruleset[i].field[1]));

    }

	map<string, int> subtable[srclevel][dstlevel];
	string str, str1, str2;
	for(k = 0; k != srclevel; k++){
		for(j = 0; j != dstlevel; j++)
		{
			its1 = field1[k].begin();
			while(its1 != field1[k].end()){
				str1 = *its1;
				its2 = field2[j].begin();
				while(its2 != field2[j].end()){
					str2 = *its2;
					str = str1 + ' ' + str2;

					itm = table.find(str);
					if(itm != table.end())
					{
						subtable[k][j].insert(make_pair(str,1));
						table.erase(itm);
					}

					its2++;
				}// end of inner while
				its1++;
			}// end of outer while
		} //end of inner for loop
	}// end of outer for loop

	for(k = 0; k != srclevel; k++){
			for(j = 0; j != dstlevel; j++)
				if(subtable[k][j].size() != 0)
					count++;
	}//for loop

//end partition part, number of subset is stored in "count"
//generate sub rule set

	subset sset[count];
	i = 0;
	for(k = 0; k != srclevel; k++)
		for(j = 0; j != dstlevel; j++){
			if(subtable[k][j].size() != 0){
				sset[i].table = subtable[k][j];
				sset[i].nl1 = k;
				sset[i].nl2 = j;
				i++;
			}
		}
	cout<<"subset number is "<<i<<endl;

	srctree.initial_p2(sset,srcip,numrules,count,0);
	dsttree.initial_p2(sset,dstip,numrules,count,1);

//	node *temp = NULL;
//	for (i = 0; i != numrules; i++){
//		temp = dsttree.lpm1(ruleset[i].field[1]);
//		int m = 0;
//		for(m = 0; m != count; m++)
//			cout<<temp->nlt[m];
//		cout<<endl;
//	}


//****************bloom filter testing part start********************//
//initializing the bloom filter

	vector<bloom_filter *> filters;
	filters.reserve(count);

	for(i = 0; i != count; i++){
		bloom_parameters parameters;
		parameters.projected_element_count = sset[i].table.size();
		parameters.false_positive_probability = 0.0001;
		parameters.random_seed = 0xA5A5A5A5;
		if (!parameters)
		   {
		      std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
		      return 1;
		   }

		parameters.compute_optimal_parameters();
		bloom_filter *filter = new bloom_filter(parameters);

		//map<string, int>::iterator it;
		//for(it = sset[i].table.begin(); it != sset[i].table.end(); ++it){
		for(const auto& tup:sset[i].table){
			filter->insert(tup.first);
		}
		filters.push_back(move(filter));

	}

//bloom filter testing part end
	unsigned sip, dip;

	sip = 3237374980;
	dip = 3432561747;

	string sip_prefix, dip_prefix;
	node *sip_ptr, *dip_ptr;

	sip_prefix = srctree.lookup(sip);
	sip_ptr = srctree.lpm(sip);

	dip_prefix = dsttree.lookup(dip);
	dip_ptr = dsttree.lpm(dip);

	for(i = 0; i != count; i++){
		cout<<sip_ptr->nlt[i]<<dip_ptr->nlt[i]<<endl;
	}

	for(i = 0; i != count; i++){
		string key;
		if((sip_ptr->nlt[i]) + (dip_ptr->nlt[i]) == 2)
		{
			string k1, k2;

			int tuple = sset[i].nl1;
			unsigned np = sip_ptr->np;
			int m = 0;
			if(tuple == 0)
				k1 = "*";
			else{
				for(m = 31; m != -1; m--){
					if((np & (1<<m)) != 0){
						if(tuple == 1)
							break;
						else tuple--;

					}
				} //end for loop
				k1 = sip_prefix.substr(0,(32-m));
				if(k1.length() != 32)
					k1.push_back('*');
			}//end else

			tuple = sset[i].nl2;
			np = dip_ptr->np;
			if(tuple == 0)
				k2 = "*";
			else{
				for(m = 31; m != -1; m--){
					if((np & (1<<m)) != 0){
						if(tuple == 1)
							break;
						else tuple--;

					}
				} //end for loop
				k2 = dip_prefix.substr(0,(32-m));
				if(k2.length() !=32)
					k2.push_back('*');
			}//end else

			key = k1 + ' ' + k2;

			cout<<key<<endl;
		}//end if

		if (filters[i]->contains(key)){
			cout<<"match rule: "<<sset[i].table.find(key)->first<<endl;

		}



	}//end for loop



return 0;

}

