#pragma once


struct Segmentation
{
	// Returns segmentation result from given sentence
	static std::vector<Utf8String> segment(const Utf8String& sentence);
};
