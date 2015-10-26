/*
 * tree.cc
 *
 *  Created on: Sep 23, 2015
 *      Author: Hao
 */
#include "tree.h"


#define popcnt(v)               __builtin_popcount(v)

int length(unsigned low, unsigned high) {
	int i = 0;
	while ((low & (1 << i)) == 0 && (high & (1 << i)) != 0) {
		i++;
		if (i == 32)
			break;
	}
	return i;
}


tree::tree(int n){
	num_node = n;
	root = new node();
	root->np = 0;
	root->black = 0;
	root->lc = NULL;
	root->rc = NULL;
	root->nlt = NULL;

}

void tree::insert(struct range ip_add) {

	int i;
	node *temp;
	temp = root;
	int nodecnt = 0;

	int len = 32 - length(ip_add.low, ip_add.high);
    int prefix = (ip_add.low >> (32 - len)) & ((unsigned) pow(2, len) - 1);
    i = len - 1;

	while (i >= 0) {
		if ((prefix & (1 << i)) == 0) {
			if ((temp->lc) == NULL) {
				node *newnode = new node();
				newnode->black = 0;
				newnode->np = 0;
				newnode->lc = NULL;
				newnode->rc = NULL;
				newnode->nlt = NULL;
				nodecnt++;

				temp->lc = newnode;
				temp = temp->lc;
//				temp = newnode;

			} else
				temp = temp->lc;

		} else {
			if ((temp->rc) == NULL) {
				node *newnode = new node();
				newnode->black = 0;
				newnode->np = 0;
				newnode->lc = NULL;
				newnode->rc = NULL;
				newnode->nlt = NULL;
				nodecnt++;

				temp->rc = newnode;
				temp = temp->rc;
//				temp = newnode;
			} else
				temp = temp->rc;
		}
		i--;
	}    //end while
	temp->black = 1;

//    if(temp != root)
//    	delete temp;          //it seem like that it can auto free
//	cout<<"insert one rule need node = "<<nodecnt<<endl;

}

void tree::nl_pl(struct range ip_add){
		int i = 0;
		unsigned level = 0;
		node *temp = root;

		int len = 32 - length(ip_add.low, ip_add.high);
		int prefix = (ip_add.low >> (32 - len)) & ((unsigned) pow(2, len) - 1);
		i = len - 1;

//		unsigned prefix = (ip_add.low) & ((unsigned) pow(2, 32) - 1);

		while (i >= 0) {

				if ((prefix & (1 << i)) == 0) {
						if (temp->lc != NULL)
						{
//							temp = temp->lc;
//							if (temp->black == 1)
//								level = level + (unsigned) pow(2, i);
							if(temp->lc->black == 1)
								level = level + (unsigned) pow(2, i);

							temp = temp->lc;
						}

						else break;

				} else {
					if (temp->rc != NULL)
					{
//						temp = temp->rc;
//						if (temp->black == 1)
//								level = level + (unsigned) pow(2, i);
						if(temp->rc->black == 1)
							level = level + (unsigned) pow(2, i);

						temp = temp->rc;
					}

					else break;

				}
				i--;
			}    //end while

		level = level<<(32-len);
//		cout<<"nl/pl:"<<level<<endl;
		temp->np = level;
}

int tree::level(range ip_add){
//	int i = 31;
//	unsigned level = 0;
//	struct node *temp;
//	temp = root;
//
//	int prefix = (ip_add.low) & ((unsigned) pow(2, 32) - 1);
//
//	while (i >= 0) {
//
//			if ((prefix & (1 << i)) == 0) {
//					if (temp->lc != NULL)
//						temp = temp->lc;
//					else break;
//
//			} else {
//				if (temp->rc != NULL)
//					temp = temp->rc;
//
//				else break;
//
//			}
//			i--;
//		}    //end while
	int i = 0;
	unsigned level = 0;
	node *temp = root;

	int len = 32 - length(ip_add.low, ip_add.high);
	int prefix = (ip_add.low >> (32 - len)) & ((unsigned) pow(2, len) - 1);
	i = len - 1;

//		unsigned prefix = (ip_add.low) & ((unsigned) pow(2, 32) - 1);

	while (i >= 0) {

			if ((prefix & (1 << i)) == 0) {
					if (temp->lc != NULL)
					{

						temp = temp->lc;
					}

					else break;

			} else {
				if (temp->rc != NULL)
				{

					temp = temp->rc;
				}

				else break;

			}
			i--;
		}    //end while

	level = popcnt(temp->np);
	return level;
}

string tree::lookup( unsigned add) {

	int i = 31;
	int len = 0;
	struct node *temp;
	temp = root;
	string prefix;

	if(temp->black == 1){
		len = 32;
	}

	while (i >= 0) {

		if ((add & (1 << i)) == 0) {
				if (temp->lc != NULL){
					if(temp->lc->black ==1){
						len = i;
					}
					temp = temp->lc;
				}


			else break;

		} else {
			if (temp->rc != NULL)
			{
				if(temp->rc->black ==1)
					len = i;

				temp = temp->rc;
			}

			else break;

		}

		i--;
	}    //end while


	if(len == 32){
		prefix.push_back('*');
		return prefix;
	}

	else{

	for(i = 31; i != len-1; i--){
		if ((add & (1 << i)) == 0)
			prefix.push_back('0');
				else prefix.push_back('1');

	}
	if(len != 0) prefix.push_back('*');

	return prefix;
	}   //end else
}

string tree::lookup1(range ip_add){
	int i;
	int len =  length(ip_add.low, ip_add.high);
	string prefix;

	if(len == 32){
			prefix.push_back('*');
			return prefix;
		}

		else{

		for(i = 31; i != len-1; i--){
			if ((ip_add.low & (1 << i)) == 0)
				prefix.push_back('0');
					else prefix.push_back('1');

		}
		if(len != 0) prefix.push_back('*');

		return prefix;
		}   //end else


}

node* tree::lpm(unsigned add){
	int i = 31;
	node *temp = root;
	node *lpm = NULL;

	while (i >= 0) {
		if (temp->black == 1)
			{
					lpm = temp;

			}


		if ((add & (1 << i)) == 0) {
				if (temp->lc != NULL){
					if(temp->lc->black == 1)
						lpm = temp->lc;

					temp = temp->lc;
				}

			else break;

		} else {
			if (temp->rc != NULL)
			{
				if(temp->rc->black ==1)
					lpm = temp->rc;

				temp = temp->rc;
			}

			else break;

		}

		i--;
	}    //end while

	return lpm;
}

//for nl/pl use
node* tree::lpm1(struct range ip_add) {

	int i;
	node *temp = root;

	int len = 32 - length(ip_add.low, ip_add.high);
    int prefix = (ip_add.low >> (32 - len)) & ((unsigned) pow(2, len) - 1);
    i = len - 1;

	while (i >= 0) {
		if ((prefix & (1 << i)) == 0) {
			if ((temp->lc) == NULL)
				break;
		   else
				temp = temp->lc;

		} else {
			if ((temp->rc) == NULL) {
				break;
			} else
				temp = temp->rc;
		}
		i--;
	}    //end while

	return temp;
}

//complete insert node, nl/pl bitmap, and return number of nested level
int tree::initial_p1(int numrules, range *field_rule){

	int i = 0;
	set<int> s1;

	for(i = 0; i != numrules; i++){
		insert(field_rule[i]);
	}
	for(i = 0; i != numrules; i++){
		nl_pl(field_rule[i]);
	}
	for(i = 0; i != numrules; i++){
		s1.insert(level(field_rule[i]));
	}

	i = s1.size();

	return i;
}

//construct nlt bitmap and the node point to the bit vector respectively
void tree::initial_p2(subset *sset, range *fieldrule, int numrules, int count, unsigned field){

	int i = 0;
//	bool nlt_map[numrules][count] = {0};   //NLT bitmap in source field

	bool **nlt_map = new bool *[numrules];
	for(i = 0; i != numrules; i++)
		nlt_map[i] = new bool[count];

	bool temp_map[count] = {0};           //store the "*" nlt bitmap
//
	for(i =0; i != count; i++)
		temp_map[i] = sset[i].checkin("*", field);

	string strb;
	string strb1;
	node* temp;
	unsigned np;

	for(i = 0; i != numrules; i++){

		strb = lookup(fieldrule[i].low);
		temp = lpm1(fieldrule[i]);
		np = temp->np;
		int m = 0;
		int n = 0;

//		cout<<"match id: "<<strb<<"  nested level: "<<temp->np<<endl;

		if(strb.length() == 1){
			for(m =0; m != count; m++)
				nlt_map[i][m] = sset[m].checkin(strb, field);
		}

		else{
				if(strb.back() == '*')
					strb.pop_back();

				while(n != 32){
					if((np & (1 << n)) != 0){
					    strb1 = strb.substr(0,(32-n));
//					    cout<<"n = "<<n<<"  substring = "<<strb1<<endl;
					    if(strb1.length() != 32)
					    	strb1.push_back('*');
//					    cout<<"sub match rule: "<<strb1<<endl;
						for(m = 0; m != count; m++)
							nlt_map[i][m] = sset[m].checkin(strb1,field) | nlt_map[i][m];
					}  //end if

					n++;
				} //end while

		}//end else

		for(m = 0; m != count; m++)
			nlt_map[i][m] = nlt_map[i][m] | temp_map[m];

//output bit vector for testing
//		for(m = 0; m != count; m++)
//			cout<<nlt_map[i][m];
//		cout<<endl;

		temp->nlt = nlt_map[i];

	} // end for loop


}




