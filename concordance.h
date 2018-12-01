//
//  concordance
//

#ifndef concordance_solution_h
#define concordance_solution_h

#include <iostream>
#include <map>            /* std::multimap<T> and std::map<T> (not used) */
#include <list>
#include <vector>
#include <algorithm>      /* std::transform */
#include <string>         /* not needed for Mac OSX */


class StringLenCmp {
public:
  StringLenCmp() = default;
  
  // sort by length first, and then alphabetically
  bool operator()(const std::string& a, const std::string& b) const {
    return (a.size() < b.size()) || (a.size() == b.size() && a < b); }
  
private:
  // no data
};


class concordance {
  typedef std::multimap<std::string, size_t, StringLenCmp> wordDictType;
  
public:
  concordance(size_t minwordlen=1, size_t maxwordlen=50)
  : minwordlen_(minwordlen), maxwordlen_(maxwordlen), words_at_length_() {
    for (size_t i = 0; i < MAXWORDLEN; ++i) {
      words_at_length_.push_back(0);
    }
  }
  
  void add_word(const std::string&, size_t); // TO DO
  size_t total_words() const; // TO DO
  size_t total_words(size_t length) const; // TO DO
  void print(std::ostream&) const; // TO DO
  
  void read_text(std::istream&, const std::string&); // ALREADY DONE
  
private:
  static const size_t MAXWORDLEN = 40;
  
  wordDictType word_map_; // multimap with key=word and value=page number
  size_t minwordlen_;
  size_t maxwordlen_;
  std::vector<size_t> words_at_length_; // number of words of each length
  std::list<std::string> tempWords;
  std::list<std::string> lineString; // going to be used to print out 
};

// ALREADY DONE: BREAK A LINE INTO A LIST OF WORDS
// Courtesy of Martin Broadhurst -- http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
template <class Container>
void split(const std::string& str, Container& cont, const std::string& delims = " ") {
  std::size_t curr, prev = 0;
  curr = str.find_first_of(delims);
  
  while (curr != std::string::npos) {                   // largest possible unsigned number
    cont.push_back(str.substr(prev, curr - prev));
    prev = curr + 1;
    curr = str.find_first_of(delims, prev);
  }
  
  cont.push_back(str.substr(prev, curr - prev));
}

// ALREADY DONE: READ FROM A TEXT FILE LINE BY LINE
void concordance::read_text(std::istream& in, const std::string& ignore) {
  std::string line;
  
  for (size_t i = 1; getline(in, line, '\n'); i++) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    
    typedef std::list<std::string> container;
	lineString.push_back(line);
    container words;                          // split into container, dump ignore chars
    split<container>(line, words, ignore);
    
    typename container::const_iterator cit;
    for (cit = words.cbegin(); cit != words.cend(); ++cit) { add_word(*cit, i); }
  }
  std::list<std::string>::const_iterator iter;
  iter = tempWords.begin();
  for (int i = 0; i < tempWords.size(); i++)
  {
	  std::string s;
	  s = *iter;
	  word_map_.insert(make_pair(s, s.size()));
	  iter++;
  }
}


// TO DO: add a (word, line_number) to the multimap. Do not add if the same (word, line_number) is already present
void concordance::add_word(const std::string& word, size_t line) {
	std::list<std::string>::iterator iter; // use iterator
	iter = tempWords.begin(); // start from beginning 
	for (int i = 0; i < tempWords.size(); i++)
	{
		if (*iter == word) // if it is found then just return it to be added
		{
			return;
		}
		iter++; // if not keep moving on
	}
	tempWords.push_back(word); // push to order
	
}

// TO DO: return the total number of words of lenth size
size_t concordance::total_words(size_t size) const {
	std::list<std::string>::const_iterator iter;
	int retval = 0; // return value of total words of each length

	iter = tempWords.begin(); // start from beginning like add function 
	for (int i = 0; i < tempWords.size(); i++)
	{
		std::string s;
		s = *iter;
		if (s.size() == size)
			retval++; // if it is the same size increace the value and move on by increasing iterator.
		iter++;
	}
	return retval;
}

// TO DO: return the total number of words of all lengths
size_t concordance::total_words() const {
std::list<std::string>::const_iterator iter;
	int retval = 0;

	iter = tempWords.begin();
	for (int i = 0; i < tempWords.size(); i++)
	{
		std::string s;
		s = *iter;
		if (s.size() < 50)
			retval++;
		iter++;
	}
	return retval;
}
}

// TO DO: print list of words and line numbers as shown in example_concordances.txt in Project requirements document
void concordance::print(std::ostream& out) const {
	for (auto &i : word_map_)
	{
		std::string word;
		word = i.first;
		out << word << " : "; // used to take the iterator position and print out the word its at in the string. 
		std::list<std::string>::const_iterator iter;
		iter = lineString.begin(); 
		for (int j = 0; j < lineString.size(); j++)
		{
			std::string s;
			s = *iter;
			if (s == " ") // if s isnt empty then move on to test.
				continue;
			s = " " + s; // all functions below test to see if any pronunciation is used and wont count for.
			if ((s.find(" " + word + " ") != std::string::npos) || (s.find(" " + word + ".") != std::string::npos) 
				|| (s.find(" " + word + ",") != std::string::npos) || (s.find(" " + word + ";") != std::string::npos)
				|| (s.find(" " + word + ":") != std::string::npos) || (s.find(" " + word + "!") != std::string::npos)
				|| (s.find(" " + word + "?") != std::string::npos) || (s.find(" " + word + "\"") != std::string::npos)
				|| (s.find(" " + word + "(") != std::string::npos) || (s.find(" " + word + ")") != std::string::npos)
				|| (s.find("\"" + word + "\"") != std::string::npos) || (s.find("\"" + word + " ") != std::string::npos)
				|| (s.find("(" + word + ")") != std::string::npos) || (s.find("(" + word + " ") != std::string::npos))
			{
				out << j + 1 << " "; // counts how many time it shows up
			}				
			iter++; // keep moving on in iterator
		}
		out <<std::endl;
	}		
	
}

#endif /* concordance_solution_h */
