#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <SSVUtils/SSVUtils.hpp>
#include <SSVUtilsJson/SSVUtilsJson.hpp>

inline bool isVowel(char mC) noexcept
{
	assert(ssvu::isAlphabetical(mC));	
	mC = ssvu::toLowercase(mC);
	return mC == 'a' || mC == 'e' || mC == 'i' || mC == 'o' || mC == 'u';
}
inline bool isConsonant(char mC) noexcept { return !isVowel(mC); }

SSVU_TEST(VowelConsonantTest)
{
	SSVUT_EXPECT(isConsonant('b'));
	SSVUT_EXPECT(isConsonant('B'));
	SSVUT_EXPECT(isConsonant('c'));
	SSVUT_EXPECT(isConsonant('C'));
	SSVUT_EXPECT(isConsonant('z'));
	SSVUT_EXPECT(isConsonant('Z'));
	SSVUT_EXPECT(isVowel('a'));
	SSVUT_EXPECT(isVowel('A'));
	SSVUT_EXPECT(isVowel('u'));
	SSVUT_EXPECT(isVowel('U'));
}
SSVU_TEST_END();

inline void syllabize(std::vector<std::string>& mTarget, const std::string& mWord)
{
	auto itr(std::begin(mWord));
	auto itrEnd(std::end(mWord));
	auto valid([&itr, &itrEnd]() -> bool { return itr != itrEnd; });

	while(valid())
	{
		std::string currentSyllable;

		// CASE: beginning VC
		if(valid() && itr == std::begin(mWord) && isVowel(*itr) && isConsonant(*(itr + 1)))
		{
			currentSyllable += *itr++;
			mTarget.emplace_back(currentSyllable);
			continue;
		}

		// CASE: C...V
		if(valid() && isConsonant(*itr))
		{
			while(valid() && isConsonant(*itr))
			{
				currentSyllable += *itr++;

				if(valid() && isVowel(*itr))
				{
					currentSyllable += *itr++;

					if(valid() && isConsonant(*itr)) 
					{
						char lastConsonant(*itr); 	
						if(itr + 1 < itrEnd && *itr == lastConsonant && *(itr + 1) == lastConsonant) 
						{
							currentSyllable += *itr++;				 
						}
					}

					break;				
				}
			}
		}

		// CASE: V
		else if(valid() && isVowel(*itr))
		{
			currentSyllable += *itr++;				
		}

		mTarget.emplace_back(currentSyllable);
	}
}

inline std::vector<std::string> getSyllabized(const std::string& mWord) 
{
	std::vector<std::string> result;
	syllabize(result, mWord);
	return result;
}

SSVU_TEST(SyllabizeTests)
{
	{
		auto vec(getSyllabized("estate"));		
		SSVUT_EXPECT(vec[0] == "e");
		SSVUT_EXPECT(vec[1] == "sta");
		SSVUT_EXPECT(vec[2] == "te");

	}

	{
		auto vec(getSyllabized("casa"));
		SSVUT_EXPECT(vec[0] == "ca");
		SSVUT_EXPECT(vec[1] == "sa");
	}

	{
		auto vec(getSyllabized("banana"));
		SSVUT_EXPECT(vec[0] == "ba");
		SSVUT_EXPECT(vec[1] == "na");
		SSVUT_EXPECT(vec[1] == "na");
	}

	{
		auto vec(getSyllabized("magia"));
		SSVUT_EXPECT(vec[0] == "ma");
		SSVUT_EXPECT(vec[1] == "gi");
		SSVUT_EXPECT(vec[2] == "a");
	}

	{
		auto vec(getSyllabized("catafratto"));
		SSVUT_EXPECT(vec[0] == "ca");
		SSVUT_EXPECT(vec[1] == "ta");
		SSVUT_EXPECT(vec[2] == "frat");
		SSVUT_EXPECT(vec[3] == "to");
	}
}
SSVU_TEST_END();

inline std::string getCmdOutput(const std::string& mStr)
{
	std::string result;
	std::string file;
	FILE* pipe{popen(mStr.c_str(), "r")};
	char buffer[10000];
	while(fgets(buffer, sizeof(buffer), pipe) != NULL)
	{
		file = buffer;
		result += file.substr(0, file.size() - 1);
	}
	pclose(pipe);

	return result;
}

inline std::string getRndImgUrlFromJson(const std::string& mGoogleResults)
{
	auto obj(ssvuj::getFromString(mGoogleResults));
	const auto& responseData(ssvuj::getObj(obj, "responseData"));
	const auto& results(ssvuj::getObj(responseData, "results"));
	auto resultCount(ssvuj::getObjSize(results));
	const auto& rndResult(ssvuj::getObj(results, ssvu::getRnd(0ul, std::min(4ul, resultCount))));
	
	return ssvuj::getExtr<std::string>(rndResult, "unescapedUrl");
}

inline std::string getExtension(std::string mPath)
{
	if(ssvu::beginsWith(mPath, '.')) mPath.erase(std::begin(mPath));
	auto extBegin(mPath.find_last_of('.'));
	std::string toSanitize{extBegin == std::string::npos ? "" : mPath.substr(extBegin, mPath.size() - extBegin)};	
	return toSanitize.substr(0, mPath.find_first_of('?'));
}

int main() 
{
	SSVU_TEST_RUN_ALL();

	

	ssvu::lo("Dictionary") << "Loading dictionary" << std::endl;

	std::vector<std::string> dictionary;	
	std::ifstream dictFile("/media/veeData/Temp/itaDict.txt");
	std::string temp;
	while(dictFile >> temp) dictionary.emplace_back(std::move(temp));

	ssvu::lo("Dictionary") << "Done" << std::endl;



	std::srand(std::time(0));
	auto getRndWord([&dictionary]() -> std::string { return dictionary[ssvu::getRnd(0ul, dictionary.size())]; });
	auto getSanitized([](const std::string& mStr){ return ssvu::getReplacedAll(mStr, " ", "%20"); });


	//ssvu::lo("Dictionary") << "10 random words" << std::endl;
	//for(auto i(0u); i < 10; ++i) ssvu::lo() << getSyllabized(getRndWord()) << std::endl;


	std::string curlPrefix{R"(curl 'https://ajax.googleapis.com/ajax/services/search/images?v=1.0&q=)"}; 
	auto getGoogleResult([&curlPrefix, &getSanitized](const std::string& mStr){ return getCmdOutput(curlPrefix + getSanitized(mStr) + R"(')") + "}"; });
	auto getRndImgUrl([&getGoogleResult](const std::string& mStr){ return getRndImgUrlFromJson(getGoogleResult(mStr)); });



	auto rebusWord1(getRndWord());
	auto rebusWord2(getRndWord());

	ssvu::lo() << rebusWord1 << std::endl; 
	ssvu::lo() << rebusWord2 << std::endl;

	auto url1(getRndImgUrl(rebusWord1));
	auto url2(getRndImgUrl(rebusWord2));

	auto ext1(getExtension(url1));
	auto ext2(getExtension(url2));

	auto path1("/media/veeData/Temp/img1" + ext1);
	auto path2("/media/veeData/Temp/img2" + ext2);
	std::string pathOutput{"/media/veeData/Temp/rebusOutput.png"};

	ssvu::lo() << url1 << " -> " << path1 << std::endl;
	ssvu::lo() << url2 << " -> " << path2 << std::endl;

	getCmdOutput(R"(curl ')" + url1 + R"(' > ')" + path1 + R"(')");
	getCmdOutput(R"(curl ')" + url2 + R"(' > ')" + path2 + R"(')");

	getCmdOutput(R"(montage )" + path1 + " " + path2 + R"( -tile 2x1 -geometry 1000x600 )" + pathOutput);

  	getCmdOutput("convert " + pathOutput + " -background Khaki label:'(" + ssvu::toStr(rebusWord1.size()) + ", " 
  		+ ssvu::toStr(rebusWord2.size()) + ")' -gravity Center -font Sans -pointsize 55 -append " + pathOutput);

	return 0;
}