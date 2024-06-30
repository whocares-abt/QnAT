#include <assert.h>
#include <sstream>
#include "qna_tool.h"

// #include <algorithm>

using namespace std;

// #define top_k_para 5
// #include <iostream>
// #include <vector>
// #include <string>
// using namespace std;

void merge(std::vector<std::string>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> leftArr(n1);
    std::vector<std::string> rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = vec[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = vec[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            vec[k] = leftArr[i];
            i++;
        } else {
            vec[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = rightArr[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<std::string>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);

        merge(vec, left, mid, right);
    }
}


MyDict::MyDict()
{
    data.clear();
}

void MyDict::insert(string word, double count)
{
    data.push_back(make_pair(word, count));
}

void MyDict::preprocess()
{
    // sort(data.begin(), data.end());
    int a = 0, b = 0, c = 0;
    int i = 0;
    string w;
    while (a < 26)
    {
        string s = "";
        s += 'a' + a;
        s += 'a' + b;
        s += 'a' + c;
        w = ((i < data.size()) ? data[i].first : "~");
        if (w >= s)
        {
            abc[a][b][c] = i;
            ++c;
            if (c == 26)
            {
                ++b;
                c = 0;
                if (b == 26)
                {
                    ++a;
                    b = 0;
                }
            }
        }
        else
            ++i;
    }
}

double MyDict::search(string word)
{
    int left = lower_bound(word);
    int right = upper_bound(word) - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (data[mid].first == word)
            return data[mid].second;

        else if (data[mid].first < word)
            left = mid + 1;

        else
            right = mid - 1;
    }
    return -1;
}

void MyDict::update(string word, double value)
{
    int left = lower_bound(word);
    int right = upper_bound(word) - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (data[mid].first == word)
            {data[mid].second = value; return ;}

        else if (data[mid].first < word)
            left = mid + 1;

        else
            right = mid - 1;
    }
}

Dictionary::Dictionary() { Table.resize(capacity); }
Dictionary::~Dictionary() {}
int Dictionary::hash(const string &word)
{
    int hash = 0;
    int p = 101;
    for (char c : word)
    {
        hash = (hash * p + c) % capacity;
    }
    return hash;
}
bool Dictionary::find(const string &word)
{
    int hash_val = hash(word);
    for (int i = 0; i < Table[hash_val].size(); i++)
    {
        if (Table[hash_val][i].first == word)
        {
            return true;
        }
    }
    return false;
}
void Dictionary::resize()
{
    vector<vector<pair<string, int>>> old = Table;
    Table.clear();
    Table.resize(2 * capacity + 1);
    capacity = 2 * capacity + 1;
    for (int i = 0; i < old.size(); i++)
    {
        for (int j = 0; j < old[i].size(); j++)
        {
            int hash_val = hash(old[i][j].first);
            Table[hash_val].push_back(old[i][j]);
        }
    }
}
void Dictionary::insert(const string &word)
{
    bool found = find(word);
    if (found)
    {
        int hash_val = hash(word);
        for (int i = 0; i < Table[hash_val].size(); i++)
        {
            if (Table[hash_val][i].first == word)
            {
                Table[hash_val][i].second++;
                break;
            }
        }
    }
    else
    {
        int hash_val = hash(word);
        Table[hash_val].push_back({word, 1});
        size++;
    }
    if (size > 0.7 * capacity)
        resize();
}
int Dictionary::count(const string &word)
{
    bool found = find(word);
    if (found)
    {
        int hash_val = hash(word);
        for (int i = 0; i < Table[hash_val].size(); i++)
        {
            if (Table[hash_val][i].first == word)
            {
                return Table[hash_val][i].second;
            }
        }
    }
    else
    {
        return 0;
    }
}

Map::Map() { Table.resize(capacity); }
Map::~Map() {}
int Map::hash(triple t)
{
    return (11 * t.book_code + 31 * t.paragraph_no + t.page_no) % capacity;
}
bool Map::find(triple t)
{
    int hash_val = hash(t);
    for (int i = 0; i < Table[hash_val].size(); i++)
    {
        if (Table[hash_val][i].first == t)
        {
            return true;
        }
    }
    return false;
}
void Map::resize()
{
    vector<vector<pair<triple, int>>> old = Table;
    Table.clear();
    Table.resize(2 * capacity + 1);
    capacity = 2 * capacity + 1;
    for (int i = 0; i < old.size(); i++)
    {
        for (int j = 0; j < old[i].size(); j++)
        {
            int hash_val = hash(old[i][j].first);
            Table[hash_val].push_back(old[i][j]);
        }
    }
}
void Map::insert(triple t, int idx)
{
    bool found = find(t);
    if (!found)
    {
        int hash_val = hash(t);
        Table[hash_val].push_back({t, idx});
        size++;
    }
    if (size > 0.7 * capacity)
        resize();
}
int Map::idx(triple t)
{
    bool found = find(t);
    if (found)
    {
        int hash_val = hash(t);
        for (int i = 0; i < Table[hash_val].size(); i++)
        {
            if (Table[hash_val][i].first == t)
            {
                return Table[hash_val][i].second;
            }
        }
    }
    else
    {
        return -1;
    }
}

PriorityQueue::PriorityQueue(int num, vector<Element> score_list)
{
    int sz = score_list.size();
    for (int i = 0; i < sz; i++)
    {
        heap_array.push_back(score_list[i]);
    }
    size = sz;
}

void PriorityQueue::clear()
{
    size = 0;
    heap_array.clear();
}

int PriorityQueue::get_parent(int idx)
{
    return (idx - 1) / 2;
}

int PriorityQueue::get_left(int idx)
{
    return (2 * idx + 1);
}

int PriorityQueue::get_right(int idx)
{
    return (2 * idx + 2);
}

void PriorityQueue::heapify_down(int idx)
{
    int left_idx = get_left(idx), right_idx = get_right(idx);
    Element curr_element = heap_array[idx];
    if (left_idx > size - 1 && right_idx > size - 1)
    {
        return;
    }
    else if (left_idx == size - 1)
    {
        Element left_element = heap_array[left_idx];
        if (curr_element.paragraph_score > left_element.paragraph_score)
        {
            return;
        }
        else
        {
            std::swap(heap_array[idx], heap_array[left_idx]);
            heapify_down(left_idx);
        }
    }
    else
    {
        Element left_element = heap_array[left_idx];
        Element right_element = heap_array[right_idx];
        if (curr_element.paragraph_score > left_element.paragraph_score && curr_element.paragraph_score > right_element.paragraph_score)
        {
            return;
        }
        else if (left_element.paragraph_score > right_element.paragraph_score)
        {
            std::swap(heap_array[idx], heap_array[left_idx]);
            heapify_down(left_idx);
        }
        else
        {
            std::swap(heap_array[idx], heap_array[right_idx]);
            heapify_down(right_idx);
        }
    }
}

void PriorityQueue::heapify_up(int idx)
{
    if (idx == 0)
    {
        return;
    }
    int parent_idx = get_parent(idx);
    Element curr = heap_array[idx], parent = heap_array[parent_idx];
    if (curr.paragraph_score > parent.paragraph_score)
    {
        std::swap(heap_array[idx], heap_array[parent_idx]);
        heapify_up(parent_idx);
    }
    else
    {
        return;
    }
}

void PriorityQueue::Insert(Element new_element)
{
    heap_array.push_back(new_element);
    size++;
    heapify_up(size - 1);
}

Element PriorityQueue::remove_min()
{
    std::swap(heap_array[0], heap_array[size - 1]);
    Element to_return = heap_array[size - 1];
    heap_array.pop_back();
    size--;
    heapify_down(0);
    return to_return;
}

void PriorityQueue::preprocess()
{
    for (int i = size/2; i >= 0; i--)
    {
        heapify_down(i);
    }
}

QNA_tool::QNA_tool()
{
    vector<string> common = {"a","the","of","for","to","and","in","is","on","that","by","this","with","i","you","it","or","be","are","from","at","as","your","was","what","when","which","how","where"};
    for(string w : common)
    {
        common_word.insert(w);
    }
}

QNA_tool::QNA_tool(string input_corpus){
    this->corpus = input_corpus;
    vector<string> common = {"a","the","of","for","to","and","in","is","on","that","by","this","with","i","you","it","or","be","are","from","at","as","your","was","what","when","which","how","where"};
    for(string w : common)
    {
        common_word.insert(w);
    }
}

QNA_tool::~QNA_tool()
{
}

void QNA_tool::Preprocess()
{
    mergeSort(all_words, 0, all_words.size());
    int total_words = all_words.size();
    long long freq = 0;
    string curr = all_words[0];
    for (int i = 0; i < total_words; i++)
    {
        if (all_words[i] == curr)
        {
            freq++;
            continue;
        }
        scores.insert(curr, 10000000000/(freq + 1)); // To change
        freq = 1;
        curr = all_words[i];
    }
    scores.insert(curr, freq + 1);
    scores.preprocess();
    all_words.clear();

    std::ifstream file("unigram_freq.csv");
    std::string line;
    bool boo = true;

    // Read each line from the file
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        // Parse the line using a stringstream
        std::string word;
        long long int freq;
        if (!boo)
        {
            if (std::getline(iss, word, ',') && iss >> freq)
            {
                // Process the word and frequency as needed
                // std::cout << "Word: " << word << ", Frequency: " << freq << std::endl;
                double frequency = scores.search(word);
                scores.update(word, frequency / (freq + 1));
            }
            else
            {
                cout<<"ERROR"<<endl;
            }
        }
        else
        {
            boo = false;
        }
    }
    // cout<<"preprocess ended"<<endl;
    // Here, you can store the values in your data structure or perform any other desired operations

    // ifstream word_count_file("unigram_freq.csv");
    // std::string line;
    // word_count_file >> line;

    // while (word_count_file >> line){

    //     std::string word, freq_str; int sz = line.size(), len = 0; //seg-fault somewhere here
    //     for (int i = 0; i < sz; i++){
    //         if (line[i] == ','){len++; break;}
    //         word += line[i]; len++;
    //     }
    //     for (int i = len; i < sz; i++){
    //         freq_str += line[i];
    //     }
    //     double frequency = scores.search(word); freq = stoll(freq_str);
    //     scores.update(word, frequency/(freq + 1));
    //     cout<<word<<endl;
    // }
    // cout<<"ended";
}

void QNA_tool::UpdateScores(vector<string> &tokens)
{
    // TODO: Implement priority queue for getting O(n + klog(n))
    //  Alt: Implement quick sort for getting expected O(n + klog(k))
    // cout<<"entered update"<<endl;
    int num_para = Paragraph_count.size();
    SortedScores.clear();
    int num_buckets = Paragraph_idx.Table.size();
    for (int i = 0; i < num_buckets; i++)
    {
        int chain_len = Paragraph_idx.Table[i].size();
        for (int j = 0; j < chain_len; j++)
        {
            AddElement(i, j, tokens);
        }
    }
    // cout<<"exit update"<<endl;
}

void QNA_tool::AddElement(int i, int j, vector<string> &tokens)
{
    // cout<<"addelement called"<<endl;
    pair<triple, int> para_pair = Paragraph_idx.Table[i][j];
    Element para_element;
    para_element.book_code = para_pair.first.book_code;
    para_element.page = para_pair.first.page_no;
    para_element.paragraph = para_pair.first.paragraph_no;

    int num_tokens = tokens.size();
    double para_score = 0;
    int idx = Paragraph_idx.idx(para_pair.first);
    int bonus = 0;
    for (int k = 0; k < num_tokens; k++)
    {
        // cout<<k<<endl;
        string &token = tokens[k];
        para_score += (scores.search(token))*(scores.search(token)) * Paragraph_count[idx].count(token);
        if(Paragraph_count[idx].count(token)!=0)
        {
            bonus++;
        }
    }
    para_element.paragraph_score = para_score + bonus*bonus*bonus;

    // cout<<para_element.book_code<<" "<<para_element.page<<" "<<para_element.paragraph<<" "<<para_element.paragraph_score<<endl;
    SortedScores.push_back(para_element);
    // cout<<"addelemet ended"<<endl;
}

bool in(char c, const string &s)
{
    bool boo = false;
    for (int i = 0; i < s.size(); ++i)
    {
        if (c == s[i])
        {
            boo = true;
        }
    }
    return boo;
}

string slice(const string &s, int i, int j)
{
    string ans = "";
    for (int k = i; k <= j; k++)
    {
        ans = ans + s[k];
    }
    return ans;
}

string lower(string s)
{
    string t = "";
    for (int i = 0; i < s.size(); ++i)
    {
        t += tolower(s[i]);
    }
    return t;
}

vector<string> SplitSentence(const string &sentence, const string &separators)
{
    vector<string> result;
    int start = 0;
    int end = 0;

    for (int i = 0; i < sentence.size(); ++i)
    {
        if (i == sentence.size() - 1)
        {

            end = i;
            if (in(sentence[i], separators))
                end = i - 1;

            string word = slice(sentence, start, end);
            if (word.size() != 0)
            {
                result.emplace_back(lower(word));
            }
        }
        else if (in(sentence[i], separators))
        {
            end = i - 1;
            string word = slice(sentence, start, end);
            if (word.size() != 0)
            {
                result.emplace_back(lower(word));
            }
            start = i + 1;
            end = i;
        }
        else
        {
            end++;
        }
    }

    return result;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    triple trituple;
    trituple.book_code = book_code;
    trituple.page_no = page;
    trituple.paragraph_no = paragraph;
    int num_dicts = Paragraph_count.size();
    int idx = Paragraph_idx.idx(trituple);
    vector<string> old_word = SplitSentence(sentence,separators);
    vector<string> words ;
    for(auto w : old_word)
    {
        if(common_word.count(w) == 0)
        {
            string lw = lower(w);
            words.push_back(lw);
        }
    }
    int num_words = words.size();

    if (idx == -1)
    {
        Dictionary new_dict;
        Paragraph_count.push_back(new_dict);
        idx = Paragraph_count.size() - 1;
        Paragraph_idx.insert(trituple, idx);
    }

    for (int i = 0; i < num_words; i++)
    {
        Paragraph_count[idx].insert(words[i]);
        all_words.push_back(words[i]);
    }

    return;
}

Node *QNA_tool::get_top_k_para(string question, int k)
{
    // Implement your function here

    if (!preprocessing)
    {
        Preprocess();
        preprocessing = true;
    }
    vector<string> tokens = SplitSentence(question, separators);
    UpdateScores(tokens);
    PriorityQueue Q(SortedScores.size(), SortedScores);
    Q.preprocess();
    Element p = Q.remove_min();
    // cout<<"pq made"<<endl;
    Node *head = new Node(p.book_code, p.page, p.paragraph, -1, -1);
    Node *curr_node = head;
    for (int i = 1; i < k; i++)
    {
        p = Q.remove_min();
        Node *new_node = new Node(p.book_code, p.page, p.paragraph, -1, -1);
        curr_node->right = new_node;
        new_node->left = curr_node;
        curr_node = new_node;
    }
	curr_node->right = nullptr;
	head->left = nullptr;
    return head;
}

void QNA_tool::query(string question, string filename)
{
    // Implement your function here

    // Optimization 1: Removing frequent words
    // Optimization 2: All words in a single sentence or paragraph
    // Optimization 3: Synonyms (?)
    Node* z = get_top_k_para(question,5);
    query_llm(filename,z,5,"insert key here",question);
    // Formula = Sum(fi*wi) + n*bonus1 + m*bonus2
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
{

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;

    std::string filename;

    if (this->corpus == "Mahatma Gandhi"){
        filename = "mahatma-gandhi-collected-works-volume-";
        filename += to_string(book_code);
        filename += ".txt";
    }
    else {
        if (book_code == 1){
            filename = "Day-By-Day-With-Bhagavan.txt";
        }
        else{
            filename = "Letters-from-Sri-Ramanasramam-vols-1-and-2_removed.txt";
        }
    }

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open())
    {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence))
    {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ','))
        {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos)
            {
                token = token.substr(start, end - start + 1);
            }

            // Check if the element is a number or a string
            if (token[0] == '\'')
            {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            }
            else
            {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if (
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph))
        {
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
{

    // first write the k paragraphs into different files

    Node *traverse = root;
    int num_paragraph = 0;
    // string prompt = "Learn the following paragraphs ";

    while (num_paragraph < k)
    {
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from " << this->corpus << "'s books.\nOn the basis of this, answer the following question.";
    outfile << question << endl;
    outfile << "Don't justify your answers. Use information only mentioned in the context.";
    outfile << "Elaborate your answers with refernce to the context provided. Put the information in points.";
    outfile << "Then summarise the information";

    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();

    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}


