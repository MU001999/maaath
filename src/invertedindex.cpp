#include "dse.hpp"

#include <cfloat>
#include <bitset>
#include <fstream>
#include <iostream>
#include <iterator>

#include <algorithm>

#ifdef DEBUG
#include <cstdio>
#endif // DEBUG


struct _KeywordInfo
{
    double times = 0.0;
    bool is_appeared_in_title = false;
};

static std::map<InvertedIndex::key_type, InvertedIndex::value_type> files;

static decltype(auto) get_ambiguity_section(const Utf8String &sentence)
{
    int pos_of_sen = 0, temp_pos = 0, end_of_sen = 0;
    std::map<int, int> ambiguity;

    for (int pos = 0, length; pos < (int)sentence.size() - 1;)
    {
        for (length = 7; length > 1; --length)
        {
            auto tmp = sentence.substr(pos, length);
            if (!InfoQuantity::count(tmp)) continue;
            for (int anpos = pos + length - 1, anlen; anpos > pos; --anpos)
            {
                for (anlen = 7; anpos + anlen > pos + length; --anlen)
                {
                    auto antmp = sentence.substr(anpos, anlen);
                    if (!InfoQuantity::count(antmp)) continue;
                    ambiguity[pos] = anpos + anlen;
                    anpos = pos;
                    break;
                }
            }
            break;
        }
        pos += length;
    }

    return ambiguity;
}


// TODO: return scores of files
<<<<<<< HEAD
int get_pos(std::string &filepath,key_type &kw) { //read file ,get distance
	std::ifstream file.open(filepath);
	int len = kw.size();
	if (file) {
		std::string temp;
		std::string article="";
		while (!file.eof()) 
		{
			std::getline(file, temp);
			article += temp;
		}
		key_type words(article);
		for (int i = 0; i < words.size()) {
			if (words.substr(i, len) == kw)
				file.close();
				return i;
		}
	}
	else 
	{
		file.close();
		return -1;
	}
		
}
std::map<std::string, double> InvertedIndex::cal_scores(const data_type &data)
{
	std::map<std::string, double>scores;//init scores
	data_type::iterator t = files.begin();
	int count = t->second.size();
	while (t != files.end()) // Traversing each keyword in every file
	{
		for(int i =0;i<count;i++)//every keyword ->every file
			if(t->second[i].is_appeared_in_title)
				scores[t->second[i].filepath]+=1000£»
			else
			{
				int pos = get_pos(t->second[i].filepath, t->first);
				scores[t->second[i].filepath] -= (pos +pos * t->second[i].density);//Average distance
			}
		scores[t->second[i].filepath] += t->second[i].density*InfoQuantity::get_infoquantity(t->first);//a keyword freq in a file
		t++;
	}
	return scores;
}

/*
Structure of temp file for serialization
*/

bool InvertedIndex::serialize(const std::string &filepath = "./dseii.tmp")
{
    auto fout = std::ofstream(filepath);

    if (!fout) return false;

    for (auto &kv : files)
    {
        fout << kv.first << std::endl;
        for (auto &file : kv.second)
        {
            fout << file.filepath << std::endl;
            fout << file.times << ' ' << file.density << ' ' << file.is_appeared_in_title << std::endl;
        }
        fout << std::endl;
    }

    return true;
}

bool InvertedIndex::unserialize(const std::string &filepath = "./dseii.tmp")
{
    auto fin = std::ifstream(filepath);
    
    if (!fin)
    {
        add_files();
        return false;
    }

    std::string line, kw;
    while (std::getline(fin, kw))
    {
        while (std::getline(fin, line))
        {
            if (line.empty()) break;

            double times, density;
            bool is_appeared_in_title;

            fin >> times >> density >> is_appeared_in_title;
            files[kw].push_back({ line, times, density, is_appeared_in_title });
        }
    }

    return true;
}

InvertedIndex::value_type InvertedIndex::get_fileinfos(const key_type &word)
{
    return files[word];
}

std::vector<std::string> InvertedIndex::get_filepaths(const std::vector<key_type> &keywords)
{
    data_type fileinfos, for_cal_scores;
    std::vector<std::vector<std::string>> filepaths_list;
    for (auto &kw : keywords)
    {
        fileinfos[kw] = get_fileinfos(kw);
        filepaths_list.push_back(std::vector<std::string>());
        for (auto &fileinfo : fileinfos[kw])
            filepaths_list.back().push_back(fileinfo.filepath);
    }

    std::vector<std::string> filepaths;
    // auto filepaths = and_files(filepaths_list);

    for (auto &filepath : filepaths)
        for (auto &mp : fileinfos)
            for (auto &fileinfo : mp.second)
                if (fileinfo.filepath == filepath)
                    for_cal_scores[mp.first].push_back(fileinfo);

    auto scores = cal_scores(for_cal_scores);
    std::sort(filepaths.rbegin(), filepaths.rend(), [&](const std::string &a, const std::string &b)
    {
        return scores[a] < scores[b];
    });

    return filepaths;
}

// TODO: sort by chapter order
void InvertedIndex::add_files(const std::string &folderpath)
{
    // TODO: implement add files with receiving a folder path
    serialize();
}

void InvertedIndex::add_file(const key_type &sentence, const std::string &filepath)// Add all the words in a sentence to the inverted index
{
    double alltimes = 0.0;

    std::map<key_type, _KeywordInfo> kwinfos;

    auto ambiguities = get_ambiguity_section(sentence);
    for (auto &mp : ambiguities)
    {
        for (auto &word : Segmentation::segment(sentence.substr(mp.first, mp.second - mp.first)))
        {
            alltimes += 1;
            kwinfos[word].times += 1;
            kwinfos[word].is_appeared_in_title |= false;
        }
    }

    int endpos = -1;
    for (auto &mp : ambiguities)
    {
        while (++endpos < mp.first - 1)
        {
            for (int length = 2; endpos + length < mp.first; ++length)
            {
                auto word = sentence.substr(endpos, length);
                if (!InfoQuantity::count(word)) continue;
                alltimes += 1;
                kwinfos[word].times += 1;
                kwinfos[word].is_appeared_in_title |= false;
            }
        }
        endpos = mp.second - 1;
    }
    while (++endpos < (int)sentence.size() - 1)
    {
        for (int length = 2; endpos + length < (int)sentence.size(); ++length)
        {
            auto word = sentence.substr(endpos, length);
            if (!InfoQuantity::count(word)) continue;
            alltimes += 1;
            kwinfos[word].times += 1;
            kwinfos[word].is_appeared_in_title |= false;
        }
    }

    for (auto &kwinfo : kwinfos)
    {
        auto &info = kwinfo.second;
        files[kwinfo.first].push_back({ filepath, info.times, info.times / alltimes, info.is_appeared_in_title });
    }
}
