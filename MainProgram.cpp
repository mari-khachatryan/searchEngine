#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <sstream>


std::vector<std::string> readDataFromFile(const std::string& filename) {
    std::vector<std::string> data;
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cout << ("Failed to open the file.") << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        data.push_back(line);
    }

    if (!inputFile.eof()) {
        std::cout << ("Failed to read data from the file.") << std::endl;
    }
    inputFile.close();

    return data;
}

std::vector<std::string> split(const std::string& input) {
    std::vector<std::string> result;
	unsigned int i{};
	unsigned int j{};
	result.push_back("");
	while (i < input.size()) {
		while (input[i] != ' ' && input[i] != '|' && input[i] != ',' && input[i] != '\0') {
			result[j].push_back(input[i]);
			++i;
			if (input[i] == ' ' ||  input[i] == ','  || input[i] == '|' ) {
				++j;
				result.push_back("");
				break;
			}
		}
		if (input[i] == '\0') {
			return result;
		}
		++i;
	}
	return result;
}

std::vector<std::string> cleanWords(const std::vector<std::string>& source ) {
    std::vector<std::string> result = {};
    for(int i{0}; i < source.size(); i++) {
        std::string word{};
        for(char c : source[i]) {
            if(std::islower(c))
                word.push_back(c);
            else if(std::isupper(c))
                word.push_back(std::tolower(c));
        }
        word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
        result.push_back(word);
    }
    return result;
}


bool binarySearch(const std::vector<std::string>& words,const std::string& target) {
    int left = 0;
    int right = words.size() - 1;

    while(left <= right) {
        int mid = left + (right - left)/2;
        if(words[mid] == target) {
            return true; //or mid;
        } else if(words[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return false;
}

std::string shortenedString(const std::string& str) {
    if (!str.empty()) {
        std::string shortenedString = str.substr(0, str.length() - 1);
        return shortenedString;
    }
    return "";
}

std::vector<std::string> normalizeWord(const std::string& path, const std::vector<std::string>& words) {
    std::vector<std::string> dic = readDataFromFile(path);
    std::vector<std::string> normlizeMyVector = {};
    for(std::string word : words) {
        if(binarySearch(dic, word)) {
            normlizeMyVector.push_back(word);
        } else {
            word = shortenedString(word);
            if(binarySearch(dic, word)) {
                normlizeMyVector.push_back(word);
            }
        }
    }
    return normlizeMyVector;
}


class Word {
public:
    Word(const std::string& w) : word{w} {}
    ~Word(){}
    std::string getWord() { return word; }
private:
    std::string word;
};


class Text {
public:
    Text() {}
    ~Text() {}
    std::vector<Word> getText() { return text; }
    void creatAddWords(Word& w, const std::string& dicPath) {
        std::string str = w.getWord();
        std::vector<std::string> vec = normalizeWord(dicPath, cleanWords(split(str)));
        for(std::string word : vec) {
            text.push_back(Word(word));
        }
    }
private:
    std::vector<Word> text;
};

void parsingFiles(const std::string& txt1, const std::string& txt2) {
    std::ifstream file1(txt1);
    std::ifstream file2(txt2);
    std::ofstream outputFile("db.txt");

    if (!file1.is_open() || !file2.is_open() || !outputFile.is_open()) {
        std::cout << "Failed to open the files." << std::endl;
        return;
    }
    std::unordered_map<std::string, std::string> words;

    std::string word;
    std::string str1;
    while (file1 >> word) {
        str1 += word + " ";
    }
    Word w1(str1);
    Text t1;
    t1.creatAddWords(w1, "words_alpha.txt");
    std::vector<Word> vec1 = t1.getText();
    for(auto t : vec1) {
        words[t.getWord()] += "1, ";
    }

    std::string str2;
    while (file2 >> word) {
        str2 += word + " ";
    }
    Word w2(str2);
    Text t2;
    t2.creatAddWords(w2, "words_alpha.txt");
    std::vector<Word> vec2 = t2.getText();
    for(auto t : vec2) {
        words[t.getWord()] += "2, ";
    }

    
    for (const auto& pair : words) {
        std::string word = pair.first;
        std::string fileNumber = pair.second;
        fileNumber.pop_back();
        outputFile << word << ":" << fileNumber << std::endl;
    }

    file1.close();
    file2.close();
    outputFile.close();

    std::cout << "Parsing completed. Results saved in db.txt." << std::endl;

}

void searchFromDB(const std::string& path, const std::string& word) {
    std::ifstream inputFile("db.txt");

    if (!inputFile.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::string currentWord;
        std::string fileNumbers;
        std::getline(iss, currentWord, ':');
        std::getline(iss, fileNumbers);

    if(currentWord == word) {
        if(fileNumbers.find('1') != std::string::npos) {
            std::cout << "Found in file: 1.txt" << std::endl;
        }
        if(fileNumbers.find('2') != std::string::npos) {
            std::cout << "also it found in file: 2.txt" << std::endl;
        }
    }
    }
    inputFile.close();
}

int main() {
    //parsingFiles("1.txt", "2.txt");
    std::cout << "For searching please enter word" << std::endl;
    std::cout << "If you wont to stop program please type 'q' char!!!!" << std::endl;
    std::string input;
    std::cin >> input;
    while(true) {
        if(input == "q") break;
        searchFromDB("db.txt", input);
    }
    return 0;
}