#pragma once


struct FileInfo
{
	std::string filepath;
	double times = 0.0, density = 0.0;
	bool is_appeared_in_title = false;
};


struct KeywordInfo
{
	double times = 0.0, density = 0.0;
	bool is_appeared_in_title = false;
};


struct FileInfoWithAllKeywords
{
	std::string filepath;
	std::map<Utf8String, KeywordInfo> kwinfos;

	FileInfoWithAllKeywords(const std::string& filepath);
};


class InvertedIndex
{
public:
	using key_type = Utf8String;
	using value_type = std::vector<FileInfo>;
	using data_type = std::map<key_type, value_type>;

private:
	data_type kwmappings_;
	std::string tempfilepath_;
	std::unordered_map<std::string, int> filesorder_;

	std::map<std::string, double> cal_scores_(const data_type& data);

public:
	// Constructor, recerives the serialization file
	InvertedIndex(const std::string& filepath);

	bool serialize();

	bool unserialize();

	value_type get_fileinfos(const key_type& sentence);

	std::vector<FileInfoWithAllKeywords> get_fileinfos(const std::vector<key_type>& keywords, int pagenum = 1, int perpage = 10);

	std::vector<std::string> get_filepaths(const std::vector<key_type>& keywords);

	// Add files with receiving a folder path
	void add_files(const std::string& folderpath = "./inputfiles");

	void add_file(const key_type& sentence, const std::string& filepath);
};
