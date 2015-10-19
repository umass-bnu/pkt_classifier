/*
 * subset.cc
 *
 *  Created on: Oct 7, 2015
 *      Author: Hao
 */
#include"subset.h"


subset::subset(){
	nl1 = 0;
	nl2 = 0;
}

bool subset::checkin(string str, unsigned field){
	//scrtree field = 0; dsttree field =1;
	string s, s1, s2;
	size_t pos;
	set<string> field1;
	set<string> field2;
	set<string>::iterator its;
	map<string, int>::iterator itm;

	for(itm = table.begin(); itm != table.end(); itm++){
		s = itm->first;
		pos = s.find(' ');
		s1 = s.substr(0, pos);
		s2 = s.substr(pos+1, s.back());
		field1.insert(s1);
		field2.insert(s2);
	}

	if(field == 0){
		its = field1.begin();
		while(its != field1.end()){
			if(str == *its)
				return true;
			its++;
		}

	}
	if(field == 1){
		its = field2.begin();
		while(its != field2.end()){
			if(str == *its)
				return true;
			its++;
		}

	 }

	return false;
}



