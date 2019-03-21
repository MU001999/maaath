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
void InvertedIndex::add_file(Utf8String sentence,int file_id,InfoQuantity dict)
{
	std::vector<wordmap> word_list;
	std::map<int,int> ambiguity_section;
	ambiguity_section = get_ambiguity_section(sentence,dict);
	Utf8String segment;
	std::map<int,int>::itreator t = ambiguity_section.begin();
	while(t!=ambiguity_section.end())
	{
		segment = sentence.substr(t->first,t->second-t->first+1);
		word_list = get_segmentation(segment,dict);
		for(int i = 0;i<word_list.size();i++)
		{
			Utf8String temp = word_list[i].word;
			if(temp.size()>=2)
				this->index[temp].insert(file_id);
		}
		t++;
		word_list.clear();

	}
	t = ambiguity_section.begin();
	int se_begin = 0;
	int se_end;
	while(t!=ambiguity_section.end())
	{
		if(t->first>se_begin)
		{
			se_end = t->first;
			Utf8String temp = sentence.substr(se_begin,se_end-se_begin);
			int pos = 0;
			int i;
			while(pos<temp.size())
			{
				for(i = 8;i>=2;i--)
				{
					if(dict.get_infoquantity(temp.substr(pos,i)>0)
							{
								this->index[temp.substr(pos,i)].insert(file_id);
								pos = pos+i;
								break;
							}
				}
				if(i==1)
					pos++;
			}
			
		}
		se_begin = t->second+1;
		t++;
	}

}



