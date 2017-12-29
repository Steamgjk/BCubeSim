#ifndef LINK_H
#define LINK_H
class Link
{
public:
	Link(double bw_up, double bw_down);
	double getBW_up();
	double getWL_up();
	double getBW_down();
	double getWL_down();
	bool getFailed();
	void Transfer_up(double d);
	void Transfer_down(double d);
	void setFailed(bool state);
	void setLink_up(double d);
	void setLink_down(double d);
	bool isIdle();
	~Link();
private: 
	double bandwidth_up;
	double workload_up;
	double bandwidth_down;
	double workload_down;
	bool isfailed;

};

#endif