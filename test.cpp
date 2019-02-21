#include "TextSimilarity.h"
#include<iostream>

void test()
{
	TextSimilarity ts("E:/TextSimilarity/cppjieba/cppjieba/dict/");
	TextSimilarity::wordFreq wf1 = ts.getWordFreq("E:/TextSimilarity/chachong/chachong/text1.txt");
	TextSimilarity::wordFreq wf2 = ts.getWordFreq("E:/TextSimilarity/chachong/chachong/text2.txt");
	for (const auto& w : wf1)
	{
		std::cout << ts.Utf8ToGbk(w.first) << ":" << w.second << " ";
	}
	std::cout << "\n";
	for (const auto& w : wf2)
	{
		std::cout << ts.Utf8ToGbk(w.first) << ":" << w.second << " ";
	}
	std::cout << "\n";
	TextSimilarity::wordFreq wfsum = ts.SumNum(wf1, wf2);
	for (const auto& w : wfsum)
	{
		std::cout << ts.Utf8ToGbk(w.first) << ":" << w.second<<" ";
	}
	std::cout << "\n";
	TextSimilarity::wordFreq wf3 = ts.BackWf(wfsum, wf1);
	TextSimilarity::wordFreq wf4 = ts.BackWf(wfsum, wf2);
	for (const auto& w : wf3)
	{
		std::cout << ts.Utf8ToGbk(w.first) << ":" << w.second << " " ;
	}
	std::cout << "\n";
	for (const auto& w : wf4)
	{
		std::cout << ts.Utf8ToGbk(w.first) << ":" << w.second << " ";
	}
	
	std::vector<int> oneHot1;
	std::vector<int> oneHot2;
	oneHot1=Transform(wf3);
	oneHot2=Transform(wf4);
	std::cout << oneHot1 << std::endl;
	std::cout << oneHot2 << std::endl;
	double result = 0;
	result = consine(oneHot1, oneHot2);
	std::cout << result << std::endl;
}

int main()
{
	test();
	return 0;
}
