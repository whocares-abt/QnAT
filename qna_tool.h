#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
// #include "dict.h"
// #include "search.h"
// #include "Map.h"
// #include "Dictionary.h"
// #include "Sortk.h"
#include<vector>



class Dictionary
{
    public:
    Dictionary();
    ~Dictionary();
    vector<vector<pair<string,int>>> Table;
    int capacity = 4;
    int size = 0;
    void resize();
    void insert(const string& word);
    int count(const string& word);
    int hash(const string& word);
    bool find(const string& word);
};

struct triple
{
    int book_code = 0;
    int page_no = 0;
    int paragraph_no = 0;
        bool operator==(const triple& other) const {return ((book_code==other.book_code)&&(page_no==other.page_no)&&(paragraph_no==other.paragraph_no));}
};


class Map
{
    public:
    vector<vector<pair<triple,int>>> Table;
    Map();
    ~Map();
    int size = 0;
    int capacity = 4;
    void resize();
    void insert(triple t, int idx);
    int idx(triple t);
    int hash(triple t);
    bool find(triple t);
};


struct Element{
    int book_code;
    int page;
    int paragraph;
    double paragraph_score;
    bool operator<(const Element& other) const {return paragraph_score<other.paragraph_score;}
};

class PriorityQueue{
    private:
    vector<Element> heap_array;
    int size;
    int get_parent(int num);
    int get_left(int num); int get_right(int num);
    void heapify_up(int idx); 
    void heapify_down(int idx);

    public:
    PriorityQueue(int num, vector<Element> Score_list);
    ~PriorityQueue(){}
    void Insert(Element new_element); // Inserts element in O(log(n)) time
    Element remove_min(); // Removes the minimum element
    void clear(); // Clears the priority queue
    void preprocess(); // Builds the heap in O(n) time
};
class MyDict
{
private:
    vector<pair<string, double>> data;
    int abc[26][26][26];

    int lower_bound(string word)
    {
        string s = word + "###";
        if (s[2] < 'a')
        {
            s[2] = 'z';
            s[1]--;
        }
        if (s[2] > 'z')
        {
            s[2] = 'z';
        }
        if (s[1] < 'a')
        {
            s[1] = 'z';
            s[2] = 'z';
            s[0]--;
        }
        if (s[1] > 'z')
        {
            s[1] = 'z';
            s[2] = 'z';
        }
        if (s[0] > 'z')
        {
            s[0] = 'z';
            s[1] = 'z';
            s[2] = 'z';
        }
        if (s[0] < 'a')
        {
            return 0;
        }
        return abc[s[0] - 'a'][s[1] - 'a'][s[2] - 'a'];
    }
    int upper_bound(string word)
    {
        string s = word + "###";
        s[2]++;
        if (s[2] < 'a')
        {
            s[2] = 'a';
        }
        if (s[2] > 'z')
        {
            s[2] = 'a';
            s[1]++;
        }
        if (s[1] < 'a')
        {
            s[1] = 'a';
            s[2] = 'a';
        }
        if (s[1] > 'z')
        {
            s[1] = 'a';
            s[2] = 'a';
            s[0]++;
        }
        if (s[0] > 'z')
        {
            return data.size() + 1;
        }
        if (s[0] < 'a')
        {
            s[0] = 'a';
            s[1] = 'a';
            s[2] = 'a';
        }
        return abc[s[0] - 'a'][s[1] - 'a'][s[2] - 'a'];
    }

public:
    MyDict();
    void insert(string word, double count);
    void preprocess();
    double search(string word);
    void update(string word, double value);
};

class QNA_tool {

public:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    vector<string> all_words;
    MyDict scores;
    string separators = ".,-:!\"\'()?[]; @";
    Map Paragraph_idx;
    vector<Dictionary> Paragraph_count;
    bool preprocessing = false;
    vector<Element> SortedScores;
    Dictionary common_word;

    void Preprocess();
    void UpdateScores(vector<string> &tokens);
    void AddElement(int i, int j, vector<string> &tokens); // Adds the element corresponding to the paragraph in the map
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    string corpus;

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    QNA_tool(std::string corpus); // New constructor
    // You can add attributes/helper functions here
};