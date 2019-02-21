#pragma once
#include <cppjieba/jieba.hpp>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include<windows.h>
#include<fstream>
#include<iostream>
#include<algorithm>
#include<iterator>
#include<assert.h>

class TextSimilarity
{
public:
	typedef std::unordered_map<std::string, int> wordFreq;
	typedef std::unordered_set<std::string> wordSet;
	TextSimilarity(std::string dict);
	double getTextSimilarity(const char* file1, const char* file2);
	std::string GbkToUtf8(std::string src_str);
	std::string Utf8ToGbk(std::string src_str);
	wordFreq getWordFreq(const char* file);
	wordFreq SumNum(wordFreq, wordFreq);
	wordFreq BackWf(wordFreq, wordFreq);

private:

	void getStopWordTable(const char* stopWordFile);

	std::string DICT;
	std::string DICT_PATH;
	std::string HMM_PATH;
	std::string USER_DICT_PATH;
	std::string IDF_PATH;
	std::string STOP_WORD_PATH;
	cppjieba::Jieba _jieba;

	wordSet _stopWordSet;
};
bool MySort(std::pair<std::string, int>& a, std::pair<std::string, int>& b);
std::vector<std::pair<std::string, int>> Sort(TextSimilarity::wordFreq);
double consine(std::vector<int> oneHot1, std::vector<int> oneHot2);
std::vector<int> Transform(TextSimilarity::wordFreq);
