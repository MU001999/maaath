#pragma once

class InvertedIndex
{
	private:
		std::ap<Utf8String,set<int>> index;//索引中每个词id对应一个文件id列表
	public:
		InvertedIndex();
		std::set<int> get_file_list(Utf8String utf8_string);
		void add_file(Utf8String utf8_string,int file_id);//utf8_string 词

};
