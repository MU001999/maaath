#pragma once


// Struct contains filepath and one keyword's info in it
struct FileInfo
{
	std::string filepath;
	double times = 0.0, density = 0.0;
	bool is_appeared_in_title = false;
};


// Struct contains single keyword's info in one file
struct KeywordInfo
{
	double times = 0.0, density = 0.0;
	bool is_appeared_in_title = false;
};


// Struct contains filepath and all keywords' info in it
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
	std::string tempfilepath_;
	data_type kw_infos_mapping_;
	std::unordered_map<std::string, int> filesorder_;

	// Calculate scores of given files
	std::map<std::string, double> cal_scores_(const data_type& kw_infos_mapping);

public:
	// Constructor, recerives the serialization file
	InvertedIndex(const std::string& filepath);

	// Serialize to serialization file
	bool serialize();

	// Unserialize from serialization file
	bool unserialize();

	// Returns fileinfos by given keywords
	std::vector<FileInfoWithAllKeywords> get_fileinfos(const std::vector<key_type>& keywords, int pagenum = 1, int perpage = 10);

	// Returns filepaths by given keywords
	std::vector<std::string> get_filepaths(const std::vector<key_type>& keywords);

	// Add files with receiving a folder path
	void add_files(const std::string& folderpath = "./inputfiles");

	// Add all the words in a sentence to the inverted index
	void add_file(const key_type& sentence, const std::string& filepath);
};
