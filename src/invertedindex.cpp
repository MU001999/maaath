#include <cstdio>
#include <cfloat>
#include <cstdlib>
#include <dirent.h>

#include <bitset>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "utf8string.hpp"
#include "infoquantity.hpp"
#include "invertedindex.hpp"


// Temporary struct in this file
struct _KeywordInfo
{
	double times = 0.0;
	bool is_appeared_in_title = false;
};


// Returns map for ambiguity section by given sentence
static decltype(auto) _get_ambiguity_section(const Utf8String& sentence)
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

// Calculate the order of given section name as '1.5.2 <NAME>' and return the result
static int _cal_order_by_secname(const std::string& secname)
{
	int first, second, third; char tmp;
	std::stringstream ss(secname);
	ss >> first >> tmp >> second >> tmp >> third;
	return first * 10000 + second * 100 + third;
}


// Contructor of FileInfoWithAllKeywords
FileInfoWithAllKeywords::FileInfoWithAllKeywords(const std::string& filepath) : filepath(filepath) {}


std::map<std::string, double> InvertedIndex::cal_scores_(const data_type& kw_infos_mapping)
{
	std::map<std::string, double> scores; // init scores

	for (auto& kw_infos : kw_infos_mapping)
	{
		auto& infos = kw_infos.second;
		for (int i = 0, concept_pos = -1; i < (int)infos.size(); ++i)
		{
			if (infos[i].is_appeared_in_title)
			{
				scores[infos[i].filepath] += 1000;
				concept_pos = filesorder_[infos[i].filepath];
			}
			else if (concept_pos > -1)
				scores[infos[i].filepath] -= abs(filesorder_[infos[i].filepath] - concept_pos) * 100;
			
			scores[infos[i].filepath] += infos[i].times * InfoQuantity::get_infoquantity(kw_infos.first);
		}
	}

	return scores;
}

InvertedIndex::InvertedIndex(const std::string & filepath) : tempfilepath_(filepath)
{
	serialize();
}

/*
Structure of temp file for serialization
*/

bool InvertedIndex::serialize()
{
	auto fout = std::ofstream(tempfilepath_);

	if (!fout) return false;

	for (auto& kw_infos : kw_infos_mapping_)
	{
		fout << kw_infos.first << std::endl;
		for (auto& infos : kw_infos.second)
		{
			fout << infos.filepath << std::endl;
			fout << infos.times << ' ' << infos.density << ' ' << infos.is_appeared_in_title << std::endl;
		}
		fout << std::endl;
	}

	return true;
}

bool InvertedIndex::unserialize()
{
	std::ifstream fin(tempfilepath_);

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
			kw_infos_mapping_[kw].push_back({ line, times, density, is_appeared_in_title });
		}
	}

	return true;
}

std::vector<FileInfoWithAllKeywords> InvertedIndex::get_fileinfos(const std::vector<key_type> & keywords, int pagenum, int perpage)
{
	data_type kw_infos_mapping, for_cal_scores;
	std::vector<std::vector<std::string>> paths_list;
	for (auto& kw : keywords)
	{
		kw_infos_mapping[kw] = kw_infos_mapping_[kw];
		paths_list.push_back(std::vector<std::string>());
		for (auto& info : kw_infos_mapping[kw]) paths_list.back().push_back(info.filepath);
	}

	std::map<std::string, FileInfoWithAllKeywords> path_infos_mapping;
	// for (auto filepath : and_files(paths_list)) path_infos_mapping[filepath] = filepath;

	for (auto& path_infos : path_infos_mapping) for (auto& kw_infos : kw_infos_mapping) for (auto& info : kw_infos.second)
		if (info.filepath == path_infos.first)
	{
		for_cal_scores[kw_infos.first].push_back(info);
		path_infos.second.kwinfos[kw_infos.first] =
		{
			info.times, info.density, info.is_appeared_in_title
		};
	}

	std::vector<FileInfoWithAllKeywords> fileinfos;
	for (auto& path_infos : path_infos_mapping)
	{
		fileinfos.push_back(path_infos.second);
	}

	auto scores = cal_scores_(for_cal_scores);
	std::sort(fileinfos.rbegin(), fileinfos.rend(), [&](const FileInfoWithAllKeywords & a, const FileInfoWithAllKeywords & b)
		{
			return scores[a.filepath] < scores[b.filepath];
		});

	return decltype(fileinfos)(fileinfos.begin() + pagenum * perpage - perpage, fileinfos.begin() + pagenum * perpage);
}

std::vector<std::string> InvertedIndex::get_filepaths(const std::vector<key_type> & keywords)
{
	data_type kw_infos_mapping, for_cal_scores;
	std::vector<std::vector<std::string>> paths_list;
	for (auto& kw : keywords)
	{
		kw_infos_mapping[kw] = kw_infos_mapping_[kw];
		paths_list.push_back(std::vector<std::string>());
		for (auto& info : kw_infos_mapping[kw]) paths_list.back().push_back(info.filepath);
	}

	std::vector<std::string> filepaths;
	// auto filepaths = and_files(filepaths_list);

	for (auto& filepath : filepaths) for (auto& kw_infos : kw_infos_mapping) for (auto& info : kw_infos.second)
		if (info.filepath == filepath) for_cal_scores[kw_infos.first].push_back(info);

	auto scores = cal_scores_(for_cal_scores);
	std::sort(filepaths.rbegin(), filepaths.rend(), [&](const std::string & a, const std::string & b)
		{
			return scores[a] < scores[b];
		});

	return filepaths;
}

void InvertedIndex::add_files(const std::string & folderpath)
{
	auto directory_pointer = opendir(folderpath.c_str());

	if (!directory_pointer)
	{
		printf("error when openning folder: %s\n", folderpath.c_str());
		exit(0);
	}

	std::vector<std::string> filenames;
	while (auto entry = readdir(directory_pointer)) filenames.push_back(entry->d_name);

	sort(filenames.begin(), filenames.end(), [&](const std::string & a, const std::string & b)
		{
			return _cal_order_by_secname(a) < _cal_order_by_secname(b);
		});

	auto prepath = folderpath + ((folderpath.back() == '/') ? "" : "/");

	for (auto& filename : filenames)
	{
		auto filepath = prepath + filename;
		std::ifstream fin(filepath);
		std::string content, line;
		for (; std::getline(fin, line); content += line + " ");
		add_file(content, filepath);
		filesorder_[filepath] = filesorder_.size();
	}

	serialize();
}

void InvertedIndex::add_file(const key_type & sentence, const std::string & filepath)
{
	double alltimes = 0.0;

	std::map<key_type, _KeywordInfo> kwinfos;

	auto ambiguities = _get_ambiguity_section(sentence);
	for (auto& mp : ambiguities)
	{
		for (auto& word : Segmentation::segment(sentence.substr(mp.first, mp.second - mp.first)))
		{
			alltimes += 1;
			kwinfos[word].times += 1;
			kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
		}
	}

	int endpos = -1;
	for (auto& mp : ambiguities)
	{
		while (++endpos < mp.first - 1)
		{
			for (int length = 2; endpos + length < mp.first; ++length)
			{
				auto word = sentence.substr(endpos, length);
				if (!InfoQuantity::count(word)) continue;
				alltimes += 1;
				kwinfos[word].times += 1;
				kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
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
			kwinfos[word].is_appeared_in_title |= Utf8String(filepath).find(word);
		}
	}

	for (auto& kwinfo : kwinfos)
	{
		auto& info = kwinfo.second;
		kw_infos_mapping_[kwinfo.first].push_back({ filepath, info.times, info.times / alltimes, info.is_appeared_in_title });
	}
}
