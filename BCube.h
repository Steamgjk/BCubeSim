#ifndef BCUBE_H
#define BCUBE_H
#include <cstdio>
#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <iostream>

#include "Switch.h"
#include "Server.h"
#include "Link.h"
using namespace std;
class BCube
{
public:
	BCube();
	BCube(int n,int k, double bw_up, double bw_down);
	void ConstructBCube(double para_size);
	int get_k();
	int get_n();
	int get_N();
	int get_W();
	int get_L();
	Switch* getSwitch(Server* s, int l);
	Server* getServer(Switch* w, int p);
	Link* getLink(Switch*w, int p );
	void printSwitch(int w_idx);
	void printServer(int s_idx);
	void printLink(int l_idx);
	double Transfer(int src, int dst, double bw);
	double Transfer(Server* src, Server* dst, double bw);
	static int getAddrVal(int addr[], int end_idx, int k_val);
	vector<int> getOneHopNeighbour(Server* src, int l);
	//Process p executes Scatter procedure at stage s with the links connecting to the n switches in level l
	// 0<=p < n
	// 0<=s <= k
	// 0<=l <= k

	double Scatter(int p, int s, int l );
	double Gather(int p, int s, int l );
	void ScatterUpdateParas(int p, int s, int recv_serv_id,  vector<int> send_serv_ids);
	void GatherUpdateParas(int p, int s, int send_serv_id,  vector<int> recv_serv_ids);
	double ParaSync();
	double TimeStatistics();
	double ParaSyncMF();
	~BCube();
private: 
	int k;
	int n;
	double bw_up;
	double bw_down;
	int para_num;
	double M;
	double para_size;
	int N;
	int W;
	int L;
	std::vector<Server*> server_vec;
	std::vector<Switch*> switch_vec;
	std::vector<Link*> link_vec;
	std::map <pair<int, int>, int> link_mp;
	
};

#endif