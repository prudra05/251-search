#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  string cleanedToken = token;
  int alphaCounter = 0;
  // Remove leading punctuation
  int index = 0;
  while (index < cleanedToken.size() && ispunct(cleanedToken[index])) {
    cleanedToken.erase(index, 1);
  }
  // Remove trailing punctuation
  index = cleanedToken.size() - 1;
  while (index < cleanedToken.size() && ispunct(cleanedToken[index])) {
    cleanedToken.erase(index, 1);
    index--;
  }
  // Convert to lowercase
  index = 0;
  while (index < cleanedToken.size()) {
    if (isalpha(cleanedToken[index])) {
      cleanedToken[index] = tolower(cleanedToken[index]);
      index++;
      alphaCounter++;
    } else {
      index++;
    }
  }
  if (alphaCounter == 0) {
    return "";
  } else {
    return cleanedToken;
  }
}

set<string> gatherTokens(const string& text) {
  stringstream ss(text);

  string token;
  set<string> tokens;
  set<string> cleanedTokens;

  while (ss >> token) {
    tokens.insert(token);
  }

  for (const auto& t : tokens) {
    string cleanedT = cleanToken(t);
    if (cleanedT != "") {
      // cout << cleanedT << endl;
      cleanedTokens.insert(cleanedT);
    }
  }

  return cleanedTokens;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  int webPageCount = 0;
  map<string, set<string>> temp = index;

  ifstream file(filename);
  if (!file.is_open()) {
    // cout << "Invalid Filename";
    return 0;
  }

  string url;
  string text;

  while (getline(file, url) && getline(file, text)) {
    set<string> tokens = gatherTokens(text);
    for (const auto& token : tokens) {
      temp[token].insert(url);
    }
    webPageCount++;
  }
  index = temp;

  file.close();
  // cout << webPageCount << endl;
  return webPageCount;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // take in first word Does not have a modifier
  stringstream ss(sentence);
  string word;
  set<string> Nullresult = {};

  ss >> word;
  set<string> result;

  // store URL's for the first word only
  string clean = cleanToken(word);

  if (index.count(clean) > 0) {
    for (string url : index.at(clean)) {
      result.insert(url);
    }
  }

  while (ss >> word) {
    if (word[0] == '+') {
      string cleanWord = cleanToken(word);
      set<string> And;

      if (index.count(cleanWord) > 0) {
        for (string url : index.at(cleanWord)) {
          if (result.find(url) != result.end()) {  // if url is in result
            And.insert(url);
          }
        }
        result = And;
      } else {
        result = Nullresult;
        return result;
      }
    }

    else if (word[0] == '-') {
      string cleanWord = cleanToken(word);

      if (index.count(cleanWord) > 0) {
        for (string url : index.at(cleanWord)) {
          if (result.find(url) != result.end()) {  // if url is in result
            result.erase(url);
          }
        }
      }
    }

    else {
      string cleanWord = cleanToken(word);

      if (index.count(cleanWord) > 0) {
        for (string url : index.at(cleanWord)) {
          result.insert(url);
        }
      } else {
        result = Nullresult;
        return result;
      }
    }
  }

  return result;
  ;
}

void searchEngine(const string& filename) {
  map<string, set<string>> index;  //---> empty

  // cout << "Stand by while building index..." << endl;
  int webPageCount = buildIndex(filename, index);

  if (webPageCount == 0) {
    cout << "Invalid filename." << endl;
  }

  cout << "Stand by while building index..." << endl;

  cout << "Indexed " << webPageCount << " pages containing " << index.size()
       << " unique terms" << endl;

  string sentence;
  cout << "Enter query sentence (press enter to quit): " << endl;
  getline(cin, sentence);

  while (sentence != "") {
    set<string> result = findQueryMatches(index, sentence);
    cout << "Found " << result.size() << " matching pages" << endl;
    for (string r : result) {
      cout << r << endl;
    }
    cout << "Enter query sentence (press enter to quit): " << endl;
    getline(cin, sentence);
  }

  cout << "Thank you for searching!" << endl;
  // TODO student
}