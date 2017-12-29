#include "Switch.h"
Switch::Switch(){

}

Switch::Switch(int idx_val, int n, int k){
	int i = 0;
	this->addr_val = idx_val;
	int n_k = 1;
	for (i = 0; i < k; i++){
		n_k *= n;
	}
	this->address[k] = idx_val/n_k;
	idx_val %= n_k;
	for(i = 0 ; i < k; i++){
		this->address[i] = idx_val % n;
		idx_val /= n; 
	}
}

int* Switch::getAddress(){
	return this->address;
}
int Switch::get_addr_val(){
	return this->addr_val;
}


Switch::~Switch(){

}