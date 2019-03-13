#include<iostream>
#include<map>
#include<vector>
using namespace std;
class Invertedindex{
	private:
		map<int,vector<int>> index; 
	public:
		Invertedindex();
		vector<int> Getlist(int id);
		void Setfile(int id,vector<int>filelist);
		void change(int id,int fileid); 
};
