#pragma once


struct InfoQuantity
{
	// Returns the infoquantity of given word
	static double get_infoquantity(const Utf8String& word);

	// Returns the number of elements matching specific word
	static bool count(const Utf8String& word);
};
