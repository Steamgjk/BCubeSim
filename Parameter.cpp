#include "Parameter.h"
#include <cstdio>
Parameter::Parameter(double M){
	this->M = M;
	this->server_ids.clear();
}
Parameter::Parameter(int para_id, double M, int n, int k, int p){
	this->M = M;
	this->para_id= para_id;
	this->setAddress(para_id, n, k, p);
}
int Parameter::getParaId(){
	return this->para_id;
}
void Parameter::addServer(int server_id){
	this->server_ids.insert(server_id);
}
int* Parameter::getAddress(){
	return this->address;
}
void Parameter::setAddress(int val, int n, int k, int p){
	int i = 0;
	for(i = 0 ; i <= k; i++){
		this->address[i] = val % n;
		val /= n; 
	}
	this->address[k+1] = p;
 }
set<int> Parameter::getServerIds(){
	return this->server_ids;
}
void Parameter::addServers(set<int>&server_ids){
	set<int>::iterator it;
	for(it = server_ids.begin(); it!= server_ids.end(); it++){
		this->addServer(*it);
	}
}
Parameter::~Parameter(){

}