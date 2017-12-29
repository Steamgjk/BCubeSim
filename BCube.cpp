#include "BCube.h"
using namespace std;
BCube::BCube(){

}
int BCube::get_n(){
	return this->n;
}
int BCube::get_k(){
	return this->k;
}
int BCube::get_N(){
	return this->N;
}
int BCube::get_W(){
	return this->W;
}
int BCube::get_L(){
	return this->L;
}
BCube::BCube(int n,int k, double bw_up, double bw_down){
	this->n = n;
	this->k = k;
	this->bw_up = bw_up;
	this->bw_down = bw_down;
	this->N = 1;
	int i = 0;
	for(i = 0; i <= k; i++){
		this->N *= n;
	}
	this->W = (k+1) * (this->N)/n;
	this->L = this->W * n;
	this->server_vec.clear();
	this->switch_vec.clear();
	this->link_vec.clear();
	this->link_mp.clear();

	this->server_vec.resize(this->N);
	this->switch_vec.resize(this->W);
	this->link_vec.resize(this->L);

}
void BCube:: ConstructBCube(double para_size){
	this->para_size = para_size;
	this->para_num = this->N * (this->k+1);
	this->M = this->para_size/(this->para_num);
	int i = 0;
	int j = 0;
	//Construct Server
	for( i = 0; i < this->N; i++){
		this->server_vec[i] = new Server(i, this->n, this->k);
		this->server_vec[i]->InitializePars(this->para_num, this->M, this->n, this->k);
		/*
		//maybe some problems!
		for(j = 0; j < this->para_num; j++){
			this->server_vec[i]->InitializePars(k, M, this->n, this->k);
		}
		**/
	}

	//Construct Switch
	int l  = 0;
	int s_n = this->N / n;
	for(l = 0; l <= this->k; l++){
		for( i = 0; i < s_n; i++){
			this->switch_vec[l*s_n + i] = new Switch(l* s_n + i , this->n, this->k);
		}
	}

	//Construct Link
	for(i = 0; i < this-> W; i++){
		for(j = 0; j < this->n; j++){
			int link_idx = i*(this->n) + j;
			this->link_vec[link_idx] = new Link(this->bw_up, this->bw_down);
			Switch* w = this->switch_vec[i];
			Server* s = this->getServer(w, j);
			this->link_mp.insert(make_pair(pair<int, int>(w->get_addr_val(), s->get_addr_val()), link_idx) );
		}
	}	
	/*
	int snum = 0;
	for(snum = 0; snum < this->server_vec.size(); snum++){
		printf("Server: %d   paraNum  %d\n", snum, this->server_vec[snum]->getParas().size() );
	}
	int test_server_id = 7;
	
	Server* serv = this->server_vec[test_server_id];
	printf("Server %d  Address %d  \n", test_server_id, serv->getAddress() );
	std::vector<Parameter*> pv  = serv->getParas();
	for(snum = 0; snum < pv.size(); snum++){
		printf("para_id %d\n", pv[snum]->getParaId());
	}
	**/
}
int BCube::getAddrVal(int addr[], int end_idx, int k_val){
	int i = 0;
	int sum = 0; 
	/*
	printf("In getAddrVal k_val =%d\n", k_val);
	for (int i = 0; i <= end_idx; ++i)
	{
		printf("%d\t", addr[i]);
	}
	printf("\n");
	**/
	for(i = end_idx; i >= 0; i-- ){
		sum =  sum * k_val + addr[i]; 
	}
	//printf("sum = %d\n",sum);
	return sum;
}
Switch* BCube::getSwitch(Server* s, int l){
	int i = 0;
	int tmp_val[this->k + 1];
	
	int cnt = 0;
	/*
	for(i = 0 ; i <= this->k; i++){
		printf("%d\t", s->getAddress()[i] );
	}
	printf("\n");
	**/
	for(i = 0 ; i <= this->k; i++){
		if( i == l){
			continue;
		}
		//printf("cnt = %d val = %d\n", cnt, s->getAddress()[i] );
		tmp_val[cnt++] = s->getAddress()[i];
	}
	tmp_val[this->k] = l;
	/*
	printf("SW\t");
	for (int i = 0; i <= this->k ; ++i)
	{
		printf("%d\t", tmp_val[i]);
	}
	printf("\n");
	**/
	int swch_idx = this->getAddrVal(tmp_val, this->k, this->n);
	//printf("swch_id x = %d\n", swch_idx );
	return this->switch_vec[swch_idx];
}
Server* BCube::getServer(Switch* w, int p){
	int l = w->getAddress()[this->k];
	int tmp_val[this->k + 1];
	int i = 0;
	int cnt = 0;
	for(i = 0; i <= this->k; i++){
		if(i == l){
			tmp_val[i] = p;
			cnt++;
		}else{
			tmp_val[cnt++] = w->getAddress()[i];
		}
	}
	int serv_idx = this->getAddrVal(tmp_val, this->k, this->n);
	return this->server_vec[serv_idx];
}

Link* BCube::getLink(Switch* w, int p ){
	int w_val = w->get_addr_val();
	int link_idx = w_val * this->n + p;
	//printf("getLink %d\n", link_idx);
	//printf("getLink\n");
	return this->link_vec[link_idx];
}
double BCube::Transfer(int src, int dst, double bw){
	//printf("src = %d  dst = %d bw = %lf\n", src, dst, bw );
	Server* s = this->server_vec[src];
	Server* d = this->server_vec[dst];
	//printf("src = %p  dst = %p \n", s, d);
	return this->Transfer(s, d, bw);
}
double BCube::Transfer(Server* src, Server* dst, double bw){
	//printf("Enter Transfer\n");
	int* src_addr = src->getAddress();
	int* dst_addr = dst->getAddress();
	int l = -1;
	int i = 0;
	for(i = 0; i <= this->k; i++){
		if(src_addr[i] != dst_addr[i]){
			l = i;
			break;
		}
	}
	//printf("l=%d\n",l );
	Switch* w = this->getSwitch(src, l);
	//printf("get switch\n");
	Link* link1 = this->getLink(w,src_addr[l]);
	//printf("get link1 \n");
	Link* link2 = this->getLink(w, dst_addr[l]);
	//printf("get Link fini\n");
	if(link1->getFailed() || link2->getFailed()){
		return -1;
	}
	else{
		//printf("else\n");
		link1->Transfer_up(bw);
		link2->Transfer_down(bw);
		/*
		printf("L=%d\n", this->L);
		//printf("%lf\n", this->link_vec[31]->getWL_up() );
		for (int mm = 0; mm < this->L; ++mm)
		{
			printf("lind_idx = %d\n", mm);
			//printf("%p\n", (this->link_vec[0]) );
		}
		**/
		return 0;
	}
	
}

void BCube::printSwitch(int w_idx){
	Switch* w = this->switch_vec[w_idx];
	int i = 0;
	printf("%d:\t",w->get_addr_val() );
	for(i = 0; i <= this->k; i++){
		printf("%d ", w->getAddress()[i] );
	}
	printf("\n");
}
void BCube::printServer(int s_idx){
	Server* s = this->server_vec[s_idx];
	int i = 0;
	printf("%d:\t",s->get_addr_val() );
	for(i = 0; i <= this->k; i++){
		printf("%d ", s->getAddress()[i] );
	}
	printf("\n");	
}
void BCube::printLink(int l_idx){
	Link* lnk = this->link_vec[l_idx];

	printf("%lf %lf %lf %lf", lnk->getBW_up(), lnk->getWL_up(),lnk->getBW_down(), lnk->getWL_down()  );

	printf("\n");	
}
vector<int> BCube::getOneHopNeighbour(Server* src, int l){
	int i = 0;
	int temp_val[this->k + 1];
	for(i = 0; i <=this->k; i++){
		temp_val[i]  = src->getAddress()[i];
	}
	std::vector<int> ret;
	ret.resize(this->n -1);
	/*
	printf("INI te_l=%d  add=%d\n", temp_val[l],src->get_addr_val() );
	for (int i = 0; i <= this->k; ++i)
	{
		printf("%d\t", temp_val[i]);
	}
	printf("\n");
	**/
	for(i = 0; i < this->n -1; i++){
		temp_val[l] = (temp_val[l]+1)% (this->n);
		/*
		for (int j = 0; j <= this->k ; ++j)
		{
			printf("t %d\t", temp_val[j] );
		}
		printf("\n");
		**/
		int server_idx = this->getAddrVal(temp_val, this->k, this->n);
		//printf("tempval = %d server_idx = %d\n",temp_val[l], server_idx);
		ret[i] = server_idx;
	}

	return ret;
	
}
/*
vector<int> BCube::getRelevantParaIds(int p, int s, int l, int loc, int para_num){
	int sta = (this->para_num) / (this->k+1) * p;
	int end = (this->para_num) / (this->k+1) * (p+1);
	vector<int> ret;
	return ret;
}
**/
double BCube::Scatter(int p, int s, int l ){
	int i = 0;
	int j = 0;
	
	int para_num = (this->para_num) / (this->k+1);
	for (i = 0; i <= s; ++i)
	{
			para_num /= this->n;
	}
	printf("Scatter p: %d  s:%d  l:%d para_num:%d\n", p,s, l, para_num);
	for(i = 0; i < this->N; i++){
		//printf("getOneHopNeighbour %d  l=%d\n", i, l);
		//在本轮scatter中，其他的one-hop-neighbour都把他们对应位置的参数块（para_num个大小为M的参数块）发给我
		std::vector<int> one_hop_neighbours = this->getOneHopNeighbour(this->server_vec[i], l);	
		/*
		int q = 0;
		for(q = 0; q < one_hop_neighbours.size(); q++){
			printf("%d\t", one_hop_neighbours[q]);
		}
		printf("\n");
		**/

		for(j = 0; j < this->n -1; j++){
			//printf("para_num = %d  M= %lf\n", para_num, this->M );
			//printf("i = %d  j = %d\n",i, one_hop_neighbours[j] );
			this->Transfer(i, one_hop_neighbours[j], para_num * this->M );
			//printf("i = %d  j = %d para_num = %d  M=%lf\n",i, one_hop_neighbours[j], para_num, this->M );

			//this->Transfer(one_hop_neighbours[j], i, para_num * this->M );

			//printf("Transfer Completed j = %d\n", j);
			//Update paras
			//this->server_vec[i]->updateParas(p,s, one_hop_neighbours, true);
			this->ScatterUpdateParas(p, s, i,  one_hop_neighbours);
		}
		/*
		printf("i= %d\n",i );
		for (int q = 0; q < this->L; ++q)
		{
			printLink(q);
		}
		getchar();
		**/
	}

	
	return 0;
}


double BCube::Gather(int p, int s, int l ){
	int i = 0;
	int j = 0;
	int para_num = 1;
	for( i  = 0; i < s; i++){
		para_num *= (this->n);
	}
	printf("Gather p: %d  s:%d  l:%d para_num:%d\n", p,s, l, para_num);
	for( i = 0; i < this->N; i++){
		//在本轮gather中，我把我负责的那全局部分参数块给其他的one-hop-neighbour （para_num个参数）
		std::vector<int> one_hop_neighbours = this->getOneHopNeighbour(this->server_vec[i], l);	
		for (j = 0; j < this->n -1; j++)
		{
			this->Transfer(i, one_hop_neighbours[j], para_num* this->M);
			//this->Transfer(one_hop_neighbours[j], i, para_num * this->M );
			//this->server_vec[i]->updateParas(p,s,one_hop_neighbours, false);
			this->GatherUpdateParas(p, s, i, one_hop_neighbours);
		}
	}
	return 0;
}
double BCube::TimeStatistics(){
	double t = 0;
	int i = 0;
	for(i = 0; i < this->L; i++){
		//printf("%lf  %lf\n", this->link_vec[i]->getWL_down(), this->link_vec[i]->getBW_down() );
		if(t <= this->link_vec[i]->getWL_down()/this->link_vec[i]->getBW_down()){
			t = this->link_vec[i]->getWL_down()/this->link_vec[i]->getBW_down();
		}
		if(t <= this->link_vec[i]->getWL_up()/this->link_vec[i]->getBW_up()){
			t = this->link_vec[i]->getWL_up()/this->link_vec[i]->getBW_up();
		}
		this->link_vec[i]->setLink_up(0);
		this->link_vec[i]->setLink_down(0);
	}
	
	printf("TimeStatistics t = %lf\n", t );
	return t;
}
double BCube::ParaSync(){
	double time_counter  = 0;
	int p = 0;
	int s = 0;
	for(s = 0; s <= this->k; s++){
		for (p = 0; p <= this->k ; p++)
		{
			this->Scatter(p,s,(p+s)%(this->k+1) );
			//getchar();

			///
		
			/*
				int snum = 0;
				for(snum = 3; snum <= 5; snum++){
					printf("Server %d\n", snum );
					printf("--------------------------\n");
					std::vector<Parameter*> pv = this->server_vec[snum]->getParas();
					int pnum = 0;
					for(pnum = 0; pnum < pv.size(); pnum++){
						printf("%d:\t", pv[pnum]->getParaId());
						set<int> ssids = pv[pnum]->getServerIds();
						set<int>::iterator sit;
						for(sit=ssids.begin(); sit != ssids.end(); sit++){
							printf("%d ", *sit );
						}
						printf("\n");
					}
				}
				getchar();
				**/
		

			////
		}
		//printf("Finishedq %d\n",s );
		time_counter += TimeStatistics();
		printf("Scatter Step %d Time: %lf\n", s, time_counter );	
		

		//getchar();
	}


	/*
	for (int i = 0; i < this->L; ++i)
	 {
	 	printLink(i);
	 }
	 **/ 
	printf("time_counter = %lf\n", time_counter );
	for(s = 0; s <= this->k; s++){
		for (p = 0; p <= this->k ; p++)
		{
		    printf("p=%d s= %d  l = %d\n",p,s, this->k - ((p+s)%(this->k+1)) );
			this->Gather(p,s,  this->k - ((p+s)%(this->k+1)) ) ;
			///
			
			/*
				int snum = 0;
				for(snum = 3; snum <= 5; snum++){
					printf("Server %d\n", snum );
					printf("--------------------------\n");
					std::vector<Parameter*> pv = this->server_vec[snum]->getParas();
					int pnum = 0;
					for(pnum = 0; pnum < pv.size(); pnum++){
						printf("%d:\t", pv[pnum]->getParaId());
						set<int> ssids = pv[pnum]->getServerIds();
						set<int>::iterator sit;
						for(sit=ssids.begin(); sit != ssids.end(); sit++){
							printf("%d ", *sit );
						}
						printf("\n");
					}
				}
				getchar();
			**/


			////
		}
		time_counter += TimeStatistics();
		printf("Gather Step %d Time: %lf\n", s, time_counter );	

	} 
	printf("time_counter = %lf\n", time_counter );


	return time_counter;
}
double BCube::ParaSyncMF(){
	double time_counter = 0;
	int p = 0;
	int s = 0;
	for(s = 0; s <= this->k; s++){
		for (p = 0; p <= this->k ; p++)
		{
			this->Scatter(p,s,( p+s)%(this->k+1) );
			//getchar();
		}
		//printf("Finishedq %d\n",s );
		time_counter += TimeStatistics();
		printf("Scatter Step %d Time: %lf\n", s, time_counter );	
		//getchar();
	}
	printf("time_counter = %lf\n", time_counter );
	for(s = 0; s <= this->k; s++){
		for (p = 0; p <= this->k ; p++)
		{
			this->Gather(p,s,(p+s)%(this->k+1));
		}
		time_counter += TimeStatistics();
		printf("Gather Step %d Time: %lf\n", s, time_counter );				
	} 
	printf("time_counter = %lf\n", time_counter );	
	return time_counter;
}
void BCube::ScatterUpdateParas(int p, int s, int recv_serv_id,  vector<int> send_serv_ids){
  		Server* recv_serv = this->server_vec[recv_serv_id];

  		int l = (p+s)%(this->k + 1);
  		//当前这个recv_serv 所负责的参数的个数
		//int para_num = recv_serv->getParas().size()/ (this->k +1);
		int para_num = this->para_num / (this->k +1);
		int i = 0;
		int j = 0;
		for(i = 0; i <= s; i++){
			para_num /= (this->n);
		}
		int loc = recv_serv->getAddress()[l];
		int temp_val[this->k+2];
		for(i= 0; i <= this->k+1; i++){
			temp_val[i] = 0;	
		}
		temp_val[this->k + 1] = p;
		for( i = 0 ; i < s; i++){
			temp_val[this->k - i] =  recv_serv->getAddress()[(i+p)%(this->k+1)];
		}
		temp_val[this->k -s] = loc;
		int sta_idx = Utils::Convert(temp_val, this->k+2, this->n);
		printf("p:%d\ts:%d\tsta:%d\tpara_num:%d\t%d-<\t",p,s,sta_idx, para_num, recv_serv_id );
  		for(int tt = 0; tt < send_serv_ids.size(); tt++){
  			printf("%d\t", send_serv_ids[tt]);
  		}
  		printf(">\n");
		for(i = sta_idx; i < sta_idx+para_num; i++){
			for(j = 0; j < send_serv_ids.size(); j++){
				//把对应节点的“对应参数”的信息拿过来,
				int send_serv_id = send_serv_ids[j];
				//Server*ss = this->server_vec[j];
				Server*ss = this->server_vec[send_serv_id];
				std::vector<Parameter*> pv = ss->getParas();
				set<int> sids =  pv[i]->getServerIds();
				////
				if(recv_serv_id == 4){

				}
				////
				recv_serv->getParas()[i]->addServers(sids);
			}
		}
}
void BCube::GatherUpdateParas(int p, int s, int send_serv_id,  vector<int> recv_serv_ids){

		Server* send_serv = this->server_vec[send_serv_id];
		int l =  ( p + this->k - s ) %(this->k + 1);
		int para_num = this->para_num / (this->k +1);
		int i = 0; 
		int j = 0;
		for( i = this->k; i >= s; i--){
			para_num /= (this->n);
		}
	    
		int loc = send_serv->getAddress()[l];
		int temp_val[this->k + 2];
		for(i= 0; i <= this->k+1; i++){
			temp_val[i] = 0;	
		}
		temp_val[this->k+1] = p;
		for( i = this->k; i>=s; i--){
			temp_val[i] = send_serv->getAddress()[ ( (this->k) -  i + p ) %(this->k+1)];
		}
		//Redundant
		/*
		if(s>0){
			temp_val[s-1] = loc;	
		}
		**/
		
			
		int sta_idx = Utils::Convert(temp_val, this->k+2, this->n);
		if(p == 1){
	    	printf("%d-<\t",send_serv_id);
	    	for (int i = 0; i < recv_serv_ids.size(); ++i)
	    	{
	    		printf("%d\t", recv_serv_ids[i] );
	    	}
	    	printf(">\tpara:%d\tsta=%d\n", para_num,sta_idx);
	    }

		for(i = sta_idx; i < sta_idx + para_num; i++){
			for(j = 0; j < recv_serv_ids.size(); j++){
				//this->p_vec[i]->addServer(server_ids[j]);
				//把这个server对应参数的server_ids,给其他one-hop neighbour

				int recv_id = recv_serv_ids[j];
				if(send_serv_id == 3){
					printf("s3  recv=%d  i=%d\n", recv_id,i );
				}
				std::vector<Parameter*> pv = send_serv->getParas();
				set<int> sids = pv[i]->getServerIds();
				this->server_vec[recv_id]->getParas()[i]->addServers(sids);
			}
		}
}
BCube::~BCube(){

}