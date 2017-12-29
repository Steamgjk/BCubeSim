#ifndef PARAMETER_H
#define PARAMETER_H
#include <set>
#define ADDR_CAP 50
using namespace std;
class Parameter
{
public:
	Parameter(double M);
	Parameter(int idx, double M, int n, int k, int p);
	void addServer(int server_id);
	int* getAddress();
	int getParaId();
	void setAddress(int val, int n, int k, int p);
	set<int> getServerIds();
	void addServers(set<int>&server_ids);
	~Parameter();
private:
	double M; //每“块”参数的大小，和Link的BW属性对应起来
	int para_id;
	int address[ADDR_CAP];
	set<int> server_ids; //这个parameter是由那几个server的对应位置的信息汇聚成的
};

#endif