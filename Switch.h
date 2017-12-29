#ifndef SWITCH_H
#define SWITCH_H
#define ADDR_CAP 50
#include <cstdio>
class Switch
{
public:
	Switch();
	Switch(int idx_val, int n, int k);
	int* getAddress();
	int get_addr_val();
	~Switch();
private:
	int address[ADDR_CAP];
	int addr_val;
};



#endif