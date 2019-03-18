#include<map>
#include<vector>
#include<set>
class InvertedIndex
{
	private:
		map<int,set<int>> index;//索引中每个词id对应一个文件id列表
	public:
		InvertedIndex();
		set<int> get_file_list(int id);
		void add_file(int id,int fileid);
		map<int,int> get_word_interval(utf8 file,int fileid,map<string,double>word);//这里的file utf8类
		vector<int> get_best_interval(int nowpos,vector<vector<int>>& interval_list,wordfreq word);//word 词频字频类

};
