#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>    // std::sort
#define K 26
using namespace std;

struct Vertex {
    int next_vert[K]; //указатель на вершину, в которую ведет ребро, если не ведет -1
    bool flag; // последняя посещенная вершина
    char value;//символ, по которому мы пришли
    int parent; // вершина родитель
    int transition[K]; // массив переходов
    int suff_link; // суффиксная ссылка  
    int compressed_suff_link; //сжатая суффиксная ссылка
    int length_pat; //длина шаблона
    vector <int> num_pat; //номер шаблона
}; 

void add_string (const string& pattern, int num_pattern, vector <Vertex> &bohr){
    Vertex symb;
    int temp_vert = 0;
    for (int i = 0; i<pattern.length(); i++ ){
        int s = pattern[i] -'A';
        if (bohr[temp_vert].next_vert[s] == -1){ 
            for (int j = 0; j < K; j++){
                symb.next_vert[j] = -1;
                symb.transition[j] = -1;
            }
            symb.flag = false;
            symb.value = pattern[i]; 
            symb.suff_link = 0;
            symb.compressed_suff_link = 0;
            symb.parent = temp_vert;
            bohr.push_back(symb);
            bohr[temp_vert].next_vert[s] = bohr.size()-1;
        }
       temp_vert = bohr[temp_vert].next_vert[s];
    }
    bohr[temp_vert].flag = true;
    bohr[temp_vert].length_pat = pattern.length();
    bohr[temp_vert].num_pat.push_back(num_pattern);
}

int get_suff_link (int v, vector <Vertex> &bohr);

int get_link(int v, char s_char, vector <Vertex> &bohr){ //вычисление перехода
    int s = s_char - 'A';
    if (bohr[v].transition[s] == -1){
        if (bohr[v].next_vert[s]!= -1)
            bohr[v].transition[s] = bohr[v].next_vert[s]; 
        else if (v == 0) 
             bohr[v].transition[s] = 0;
        else 
             bohr[v].transition[s] = get_link(get_suff_link(v, bohr), (char)(s+'A'), bohr);
    }
    return bohr[v].transition[s];
}

int get_suff_link (int v, vector <Vertex> &bohr){ //получение суффиксной ссылки
    if (bohr[v].suff_link == 0){
        if ( bohr[v].parent == 0 || v == 0)
            bohr[v].suff_link = 0;
        else 
            bohr[v].suff_link = get_link(get_suff_link(bohr[v].parent, bohr), bohr[v].value, bohr);
    }
    return bohr[v].suff_link;
}

int get_compress_suff_link( int v,  vector <Vertex> &bohr){ //вычисление сжатой суфф ссылки 
    if (bohr[v].compressed_suff_link == 0){
        if (bohr[get_suff_link(v, bohr)].flag == true)
            bohr[v].compressed_suff_link = get_suff_link(v, bohr);
        else if (get_suff_link(v, bohr) == 0)
            bohr[v].compressed_suff_link = 0;
        else 
            bohr[v].compressed_suff_link = get_compress_suff_link (get_suff_link(v, bohr), bohr);
    }
    return bohr[v].compressed_suff_link;
}

    struct Answer {
        int num;
        int pat;
    };

int main() {
    string text;
    int num_pat;
    string pattern;

    vector<Answer> result;
    vector<Vertex> bohr; // бор
    Vertex root;
    for (int i = 0; i<K; i++){
        root.next_vert[i]=-1;
         root.transition[i] = -1;
    }
    root.flag = false;
    root.value ='#';
    root.suff_link = -1;
    root.compressed_suff_link = -1;
    root.parent = 0;
    bohr.push_back(root);

    cin >> text;
    cin >> num_pat;
    for (int i = 0; i < num_pat; i++){
        cin >> pattern;
        add_string (pattern, i+1, bohr);
    }

    int v = 0;
    for (int i = 0; i< text.length(); i++){
        v = get_link(v, text[i], bohr);
           int k = v;
           while (k > 0){
               if (bohr[k].flag == true){
                    for (size_t r = 0; r<bohr[k].num_pat.size(); r++){
                         Answer a;
                         a.num = i - bohr[k].length_pat + 2;
                         a.pat = bohr[k].num_pat[r];
                         result.push_back(a);
                    }
               }
                k = get_compress_suff_link(k, bohr);
           }
    }

   stable_sort(result.begin(), result.end(), [](const Answer &a, const Answer &b) { return a.pat < b.pat; });
   stable_sort(result.begin(), result.end(), [](const Answer &a, const Answer &b) { return a.num < b.num; });
   for (int i = 0; i< result.size(); i++ )
        cout << result[i].num << " " << result[i].pat << endl;
}