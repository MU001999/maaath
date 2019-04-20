#pragma once


class Utf8String;

struct InfoQuantity
{
	// returns the infoquantity of given word
	static double get_infoquantity(const Utf8String& word);

	// returns the number of elements matching specific word
	static bool count(const Utf8String& word);
};
