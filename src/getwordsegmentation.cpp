#include"dse.hpp"

#include<map>
#include<set>
#include<bitset>

#include"infoquantity.cpp"
#include"utf8string.cpp"
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
			
			freq = dict.get_infoquantity(temp)
			
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
				freq = sentence.find(temp);
				if(freq>0)
				{
					if(temp_pos+i-1>end)
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
void choice_word(std::vector<wordmap>word,std::bitset<32>bits,int count,std::vector<wordmap>&interval)
{
	for(int i = 0;i<count;i++){
		if(bits[i]){
			interval.push_back(word[count-i-1]);
		}
	}
}
bool is_overlapping(std::vector<wordmap>wd)
{
	int word_end_pos = -1;
	for(int i = 0;i<wd.size();i++)
	{
		string m = wd[i].word;
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
{//一个歧义区间 
	wordmap word_map;
	Utf8String temp;
	vector<wordmap> word_map_dict;
	for(int i = 0;i<sentence.size()-1;i++){
		if(sentence.size()-i==2){
			temp = sentence.substr(i,2);
			if(dict.get_infoquantity(temp)>0)
			word_map.pos =i;
			word_map.word = temp;
			word_map_dict.push_back(word_map);
			break;	
		}
		for(int j =7;j>=2;j--)
		{
			temp = sentence.substr(i,j);
			if(dict.get_infoquantity(temp)>0)
			{
				word_map.pos = i;
				word_map.word = temp;
				word_map_dict.push_back(word_map);
			}
			
		}
	}
	//word_map_dict 所有可能的词 
	int count = word_map_dict.size();//开始全排列 
	int arrange = BIT; 
	arrange = arrange<<count;//设置全排列的bit 
	std::vector<wordmap>temp_segment;
	std::vector<wordmap>best_segment;
	for(int i =0;i<sentence.size();i++){
		wordmap tem;
		tem.word=sentence.substr(i,1);
		tem.pos_in_sentence = i; 
		best_segment.push_back(tem);
	}//最坏情况 
	double freq = get_infoquantity(best_segment,dict);
	while(arrange<BIT)
	{
		std::bitset<32> bits(arrange);
		temp_segment.clear();
		choice_word(word_map_list,bits,count,temp_segment);
		if(is_overlapping(temp_segment))
		{
			arrange++;
			continue;
		}
		else//将无冲突区间以及单字加入到temp_segment 
		{
			wordmap tem;
			int pos_of_sen = 0;
			int temp_size = sentence.size();
			for(int i =0;i<temp_size;i++){
				while(pos_of_sen<temp_segment[i].pos){
					tem.word = sentence.substr(pos_of_sen,1);
					tem.pos_in_sentence = pos_of_sen;
					temp_segment.push_back(tem);
					pos_of_sen++;
				}
				pos_of_sen += temp_segment[i].word.size();
			}
			while(pos_of_sen<sentence.size())
			{
				tem.word = sentence.substr(pozz,1);
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
