#include"dse.hpp"

#include"invertedindex.hpp"
#include"getwordsegmentation.cpp"
InvertedIndex::InvertedIndex(){};
std::set<int> InvertedIndex::get_file_list(Utf8String word)
{
	std::map<Utf8String,std::set<int>>::iterator t;
	t = this->index.find(word);
	if(t!=this->index.end())
	return temp->second;
	else
	{
			std::set<int> empty_file_list;
			empty_file_list.clear();
			return empty_file_list;	
	} 
}
void InvertedIndex::add_file(Utf8String utf8_string,int file_id)
{
	std::vector<wordmap> word_list;
	word_list = get_segmentation(Utf8String sentence,InfoQuantity dict);
	std::set<int>::iterator t;
	for(int i = 0;i<word_list.size();i++)
	{	
		this->index[word_list[i].word].insert(file_id);
	}
}



