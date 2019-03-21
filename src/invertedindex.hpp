#pragma once

class InvertedIndex
{
	private:
		std::map<Utf8String, std::set<int>> index;//索引中每个词id对应一个文件id列表

	public:
		InvertedIndex();
		std::set<int> get_file_list(Utf8String utf8_string);
		void add_file(Utf8String utf8_string, int file_id, InfoQuantity dict);//utf8_string 词
};
