#include "Link.h"
Link::Link(double bw_up, double bw_down){
	this->bandwidth_up = bw_up;
	this->workload_up  = 0;
	this->bandwidth_down = bw_down;
	this->workload_down  = 0;
	this->isfailed = false;
}
void Link::setFailed(bool state){
	this->isfailed = state;
}
bool Link::getFailed(){
	return this->isfailed;
}
double Link::getBW_up(){
	return this->bandwidth_up;
}
double Link::getWL_up(){
	return this->workload_up;
}
void Link::setLink_up(double d){
	this->workload_up = d;
}
void Link::Transfer_up(double d){
	
	/*
	double ret = 0;
	if(this->workload_up >= d ){
		this->workload_up -= d;
		return d;
	}
	else{
		ret = this->workload_up;
		this->workload_up = 0;
		return ret;
	}
	**/
	this->workload_up += d;
}
double Link::getBW_down(){
	return this->bandwidth_down;
}
void Link::setLink_down(double d){
	this->workload_down = d;
}
double Link::getWL_down(){
	return this->workload_down;
}
void Link::Transfer_down(double d){
	
	/*
	double ret = 0;
	if(this->workload_down >= d ){
		this->workload_down -= d;
		return d;
	}
	else{
		ret = this->workload_down;
		this->workload_down = 0;
		return ret;
	}
	**/
	this->workload_down += d;
}
bool Link::isIdle(){
	return (this->workload_up < 1e9  &&  this->workload_down < 1e9);
}
Link::~Link(){

}
