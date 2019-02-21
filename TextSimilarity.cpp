#include "TextSimilarity.h"

using namespace std;
const char* const DICT_PATH = "E:/TextSimilarity/cppjieba/cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "E:/TextSimilarity/cppjieba/cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "E:/TextSimilarity/cppjieba/cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "E:/TextSimilarity/cppjieba/cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "E:/TextSimilarity/cppjieba/cppjieba/dict/stop_words.utf8";

TextSimilarity::TextSimilarity(string dict)
	:DICT(dict)
	, DICT_PATH(dict+"jieba.dict.utf8")
	, HMM_PATH(dict + "hmm_model.utf8")
	, USER_DICT_PATH(dict + "user.dict.utf8")
	, IDF_PATH(dict + "idf.utf8")
	, STOP_WORD_PATH(dict + "stop_words.utf8")
	, _jieba(DICT_PATH,
			HMM_PATH,
			USER_DICT_PATH,
			IDF_PATH,
			STOP_WORD_PATH)
{
	getStopWordTable(STOP_WORD_PATH.c_str());
}
TextSimilarity::wordFreq TextSimilarity::getWordFreq(const char* filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "open file:" << filename << "failed!!" << endl;
		return wordFreq();
	}
	string line;
	wordFreq wf;
	while (!fin.eof())
	{
		getline(fin, line);
		//GBK->UTF8
		line = GbkToUtf8(line);
		vector<string> words;
		//对当前文本进行分词
		_jieba.Cut(line, words, true);
		//统计词频
		for (const auto& e : words)
		{
			//去掉停用词
			if (_stopWordSet.count(e)>0)
			{
				continue;
			}
			else
			{
				//统计词频
				if (wf.count(e)>0)
				{
					wf[e]++;
				}
				else
				{
					wf[e] = 1;
				}
			}
		}
	}
	return wf;
}

void TextSimilarity::getStopWordTable(const char* stopWordFile)
{
	ifstream fin(stopWordFile);
	if (!fin.is_open())
	{
		cout << "open file:" << stopWordFile << "failed!!" << endl;
		return;
	}
	string line;
	wordFreq wf;
	while (!fin.eof())
	{
		getline(fin, line);
		//UTF8
		_stopWordSet.insert(line);
	}
	fin.close();
}

std::string TextSimilarity::GbkToUtf8(std::string src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

std::string TextSimilarity::Utf8ToGbk(std::string src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

bool MySort(std::pair<std::string, int>& a, std::pair<std::string, int>& b)
{
	return a.second > b.second;
}

std::vector<std::pair<std::string, int>> Sort(TextSimilarity::wordFreq wf)
{
	std::vector<std::pair<std::string, int>> tmp;
	for (auto& i : wf)
		tmp.push_back(i);
	std::sort(tmp.begin(), tmp.end(), MySort);
	return tmp;
}

TextSimilarity::wordFreq TextSimilarity::SumNum(TextSimilarity::wordFreq wf1, TextSimilarity::wordFreq wf2)
{
	TextSimilarity::wordFreq wdsum;
	wdsum.insert(wf1.begin(),wf1.end());
	wdsum.insert(wf2.begin(), wf2.end());
	for (std::unordered_map<std::string, int>::iterator i = wf1.begin(); i != wf1.end(); ++i)
	{
		for (std::unordered_map<std::string, int>::iterator j = wf2.begin(); j != wf2.end(); ++j)
		{
			if ((*i).first == (*j).first)
			{
				wdsum.insert(std::pair < std::string, int>((*i).first, ((*i).second + (*j).second)));
				wdsum[(*i).first] = (*i).second + (*j).second;
			}
		}
	}
	return wdsum;
}
TextSimilarity::wordFreq TextSimilarity::BackWf(TextSimilarity::wordFreq wfsum, TextSimilarity::wordFreq wf1)
{
	int flag = 0;
	for (std::unordered_map<std::string, int>::iterator i = wfsum.begin(); i != wfsum.end(); ++i)
	{
		for (std::unordered_map<std::string, int>::iterator j = wf1.begin(); j != wf1.end(); ++j)
		{
			if ((*i).first == (*j).first)
			{
				flag = 1;
				wfsum[(*i).first] = (*j).second;
				break;
			}
		}
		if (flag != 1)
		{
			wfsum[(*i).first] = 0;
		}
		flag = 0;
	}
	return wfsum;
}

std::vector<int> Transform(TextSimilarity::wordFreq wf)
{
	std::vector<int> v1;
	std::vector<int> v2;
	for (const auto& w : wf)
	{
		v1.push_back(w.second);
	}
	return v1;
}

double consine(std::vector<int> oneHot1, std::vector<int> oneHot2)
{
	double modular1 = 0, modular2 = 0;
	double products = 0;
	assert(oneHot1.size() == oneHot2.size());
	for (int i = 0; i < oneHot1.size(); i++)
	{
		products += oneHot1[i] * oneHot2[i];
	}

	for (int i = 0; i < oneHot1.size(); i++)
	{
		modular1 += pow(oneHot1[i], 2);
	}
	modular1 = pow(modular1, 0.5);
	for (int i = 0; i < oneHot2.size(); i++)
	{
		modular2 += pow(oneHot1[i], 2);
	}
	modular2 = pow(modular2, 0.5);
	return products / (modular1*modular2);
}
