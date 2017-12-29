#ifndef SERVER_H
#define SERVER_H
#define ADDR_CAP 50
#include "Parameter.h"
#include "Utils.h"
#include <vector>
class Server
{
public:
	Server();
	Server(int idx_val, int n, int k);
	int* getAddress();
	int get_addr_val();
	void InitializePars(int num, int M, int n, int k);
	void updateParas(int p, int s, vector<int> server_ids, bool isScatter);
	vector<Parameter*> getParas();
	~Server();
private:
	int address[ADDR_CAP];
	std::vector<Parameter*> p_vec;
	int addr_val;
	int n; 
	int k;
	
};
#endif