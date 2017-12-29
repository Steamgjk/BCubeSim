#include <cstdio>
#include "Server.h"
Server::Server(){

}
/**
24
6 0
1 2
0 1
120
**/
Server::Server(int idx_val, int n, int k){
	int i = k;
	this->addr_val = idx_val;
	for(i = 0 ; i <= k; i++){
		this->address[i] = idx_val % n;
		idx_val /= n; 
	}
	this->n = n;
	this->k = k;
}
int* Server::getAddress(){
	return this->address;
}
int Server::get_addr_val(){
	return this->addr_val;
}
void Server::InitializePars(int num, int M, int n, int k){
	this->p_vec.resize(num);
	int i = 0;
	int p = 0;
	int share = num/(k+1);
	for (p = 0; p <= this->k; ++p)
	{
		for (i = 0; i < share; i++){
			this->p_vec[p*share+i] = new Parameter(i, M, n, k, p);
			this->p_vec[p*share+i]->addServer(this->addr_val);
		}
	}

}
void Server::updateParas(int p, int s, vector<int> server_ids, bool isScatter){

	if(isScatter){
		int l = (p+s)%(this->k + 1);
		int para_num = this->p_vec.size()/ (this->k +1);
		int i = 0;
		int j = 0;
		for(i = 0; i <= s; i++){
			para_num /= (this->n);
		}
		int loc = this->address[l];
		int temp_val[this->k+2];
		for(i= 0; i <= this->k+1; i++){
			temp_val[i] = 0;	
		}
		temp_val[this->k + 1] = p;
		for( i = 0 ; i < s; i++){
			temp_val[this->k - i] =  this->address[i];
		}
		temp_val[this->k -s] = loc;
		int sta_idx = Utils::Convert(temp_val, this->k+2, this->n);
		for(i = sta_idx; i < sta_idx+para_num; i++){
			for(j = 0; j < server_ids.size(); j++){
				//把对应节点的“对应参数”的信息拿过来,
				this->p_vec[i]->addServer(server_ids[j]);
			}
		}
	}else{
		int l =  ( p + this->k - s ) %(this->k + 1);
		int para_num = this->p_vec.size()/ (this->k +1);
		int i = 0; 
		int j = 0;
		for( i = (this->k +1); i >= s; i--){
			para_num /= (this->n);
		}
		int loc = this->address[l];
		int temp_val[this->k + 2];
		for(i= 0; i <= this->k+1; i++){
			temp_val[i] = 0;	
		}
		temp_val[this->k+1] = p;
		for( i = this->k; i>=s; i--){
			temp_val[i] = this->address[ (this->k) -  i];
		}
		//Redundant
		/*
		if(s>0){
			temp_val[s-1] = 0;	
		}
		**/
			
		int sta_idx = Utils::Convert(temp_val, this->k+2, this->n);
		for(i = sta_idx; i < sta_idx + para_num; i++){
			for(j = 0; j < server_ids.size(); j++){
				//this->p_vec[i]->addServer(server_ids[j]);
				//把这个server对应参数的server_ids,给其他one-hop neighbour
			}
		}

	}
}
vector<Parameter*> Server::getParas(){
	return this->p_vec;
}
Server::~Server(){

}