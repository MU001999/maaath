#include"dse.hpp"

#include <bitset>
#include <iterator>

#define BIT 0xffffffff


struct wordmap
{
	Utf8String word;
	int pos_in_sentence;
};

std::map<int,int> get_ambiguity_section(Utf8String sentence,InfoQuantity dict)
{
	int pos_of_sen = 0;
	int temp_pos = 0;
	int end_of_sen = 0;
	std::map<int,int>interval;
	double freq;
	while(start<file.size())
	{
		Utf8String temp;
		int i;
		for(i=8;i>1;i--)
		{

			temp = file.substr(start,i);
			freq = dict.get_infoquantity(temp);

			if(freq>0)
			{
				end_of_sen = i+pos_of_sen-1;
				break;
			}
		}
		if(i==1)
		{
			pos_of_sen++;
			continue;
		}
		temp_pos=pos_of_sen + 1;
		while(temp_pos<=end_of_sen&&temp_pos<sentence.size())
		{
			for(i =8;i>1;i--)
			{
				temp = sentence.substr(temp_pos,i);
				freq = dict.get_infoquantity(temp);
				if(freq>0)
				{
					if(temp_pos+i-1>end_of_sen)
					{
						end_of_sen = temp_pos+i-1;
						temp_pos++;
						break;
					}
				}
			}

			temp_pos++;
		}
		interval[pos_of_sen] = end_of_sen;
		pos_of_sen = end_of_sen+1;

	}
	std::map<int,int>::iterator t = interval.end();
	t--;
	if(t->second>=sentence.size())
		t->second = sentence.size();
	return interval;
}

double get_infoquantity(std::vector<wordmap>wl,InfoQuantity dict)
{
	double info = 0;
	for(int i = 0;i<wl.size();i++)
	{
		wordmap temp;
		temp = wl[i];
		info+=dict.get_infoquantity(temp.word);
	}
	return info;
}

void choice_word(std::vector<wordmap>word,std::bitset<32>bits,int count,std::vector<wordmap>&section)
{
	for(int i = 0;i<count;i++){
		if(bits[i]){
			section.push_back(word[count-i-1]);
		}
	}
}

bool is_overlapping(std::vector<wordmap>wd)
{
	int word_end_pos = -1;
	for(int i = 0;i<wd.size();i++)
	{
		auto m = wd[i].word;
		int pos = wd[i].pos_in_sentence;
		if(word_end_pos<pos)
		{
			word_end_pos = pos+m.size();
		}
		else
			return true;
	}
	return false;
}

std::vector<wordmap> get_segmentation(Utf8String sentence,InfoQuantity dict)
{
	wordmap word_map;
	Utf8String temp;
	std::vector<wordmap> word_map_dict;
	for(int i = 0;i<sentence.size()-1;i++){
		if(sentence.size()-i==2){
			temp = sentence.substr(i,2);
			if(dict.get_infoquantity(temp)>0)
			word_map.pos_in_sentence =i;
			word_map.word = temp;
			word_map_dict.push_back(word_map);
			break;
		}
		for(int j =7;j>=2;j--)
		{
			temp = sentence.substr(i,j);
			if(dict.get_infoquantity(temp)>0)
			{
				word_map.pos_in_sentence = i;
				word_map.word = temp;
				word_map_dict.push_back(word_map);
			}

		}
	}
	int count = word_map_dict.size();
	int arrange = BIT;
	arrange = arrange<<count;
	std::vector<wordmap>temp_segment;
	std::vector<wordmap>best_segment;
	for(int i =0;i<sentence.size();i++){
		wordmap tem;
		tem.word=sentence.substr(i,1);
		tem.pos_in_sentence = i;
		best_segment.push_back(tem);
	}
	double freq = get_infoquantity(best_segment,dict);
	while(arrange<BIT)
	{
		std::bitset<32> bits(arrange);
		temp_segment.clear();
		choice_word(word_map_dict, bits, count, temp_segment);
		if(is_overlapping(temp_segment))
		{
			arrange++;
			continue;
		}
		else
		{
			wordmap tem;
			int pos_of_sen = 0;
			int temp_size = sentence.size();
			for(int i =0;i<temp_size;i++){
				while(pos_of_sen < temp_segment[i].pos_in_sentence){
					tem.word = sentence.substr(pos_of_sen,1);
					tem.pos_in_sentence = pos_of_sen;
					temp_segment.push_back(tem);
					pos_of_sen++;
				}
				pos_of_sen += temp_segment[i].word.size();
			}
			while(pos_of_sen<sentence.size())
			{
				tem.word = sentence.substr(pos_of_sen,1);
				tem.pos_in_sentence = pos_of_sen;
				temp_segment.push_back(tem);
				pos_of_sen++;
			}
			if(freq>=get_infoquantity(temp_segment,dict))
			{
				best_segment.clear();
				best_segment = temp_segment;
			}
			arrange++;
		}
	}
	return best_segment;
}

InvertedIndex::InvertedIndex(){};
std::set<int> InvertedIndex::get_file_list(Utf8String word)
{
	std::map<Utf8String,std::set<int>>::iterator t;
	t = this->index.find(word);
	if(t!=this->index.end())
	return t->second;
	else
	{
			std::set<int> empty_file_list;
			empty_file_list.clear();
			return empty_file_list;
	}
}

void InvertedIndex::add_file(Utf8String sentence,int file_id, InfoQuantity dict)//将一句话中的所有的词加入到倒排索引中
{
	std::vector<wordmap> word_list;
	std::map<int,int> ambiguity_section;
	ambiguity_section = get_ambiguity_section(sentence,dict);
	Utf8String segment;
	std::map<int,int>::iterator t = ambiguity_section.begin();
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

	word_list = get_segmentation(sentence, dict);
	for(int i = 0;i<word_list.size();i++)
	{
		this->index[word_list[i].word].insert(file_id);
	}
	t = ambiguity_section.begin();
	int se_begin = 0;
	int se_end;
	while(t!=ambiguity_section.end())
	{
		if(t->first>se_begin)
		{
			se_end = t->first;
			Utf8String temp = sentence.substr(se_begin, se_end-se_begin);
			int pos = 0;
			int i;
			while(pos<temp.size())
			{
				for(i = 8;i>=2;i--)
				{
					if(dict.get_infoquantity(temp.substr(pos,i))>0)
					{
						this->index[temp.substr(pos,i)].insert(file_id);
						pos = pos+i;
						break;
					}
				}
				if(i==1) pos++;
			}

		}
		se_begin = t->second+1;
		t++;
	}

}
