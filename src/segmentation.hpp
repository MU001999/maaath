#pragma once

#include <vector>


class Utf8String;

struct Segmentation
{
	// returns segmentation result from given sentence
	static std::vector<Utf8String> segment(const Utf8String& sentence);
};
