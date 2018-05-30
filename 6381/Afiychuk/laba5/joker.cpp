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
    for (size_t i = 0; i<pattern.length(); i++ ){
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
    string pattern;
    vector <string> substr_vec; // подстроки без джокеров
    vector <int> position; //позиции этих подстрок
    char joker;
    string substring;
    size_t C[100000] = {0};

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
    cin >> pattern;
    cin >> joker;

    for (size_t i = 0; i < pattern.length(); i++){ // разбиение на подстроки без джокеров
        if (pattern[i]!= joker){
            substring.insert(substring.end(), pattern[i]);
            if ((pattern[i-1] == joker) || (i == 0))
                 position.push_back(i+1);
        }
        if (i == pattern.length()-1 || pattern[i] == joker) {
            if ((i!=0 && pattern[i-1]!= joker) || 
            (i == pattern.length()-1 && pattern[i]!= joker) ){
            substr_vec.push_back(substring);
            substring.clear();
            }
        }
   }

    for (size_t i = 0; i < substr_vec.size(); i++){ // строим бор
        add_string (substr_vec[i], i+1, bohr);
    }

    int v = 0; // считаем Ахо-Корасик
    for (size_t i = 0; i < text.length(); i++){
        v = get_link(v, text[i], bohr);
        int m = v;
        while (m > 0){
             if (bohr[m].flag == true){
                for (size_t r = 0; r<bohr[m].num_pat.size(); r++){
                     Answer a;
                    a.num = i - bohr[m].length_pat + 2;
                    a.pat = bohr[m].num_pat[r];
                    result.push_back(a);
                    if ((a.num - position[a.pat-1] + 1 > 0)&&(a.num + pattern.length() - position[a.pat-1] <= text.length()))
                        C[a.num - position[a.pat-1] + 1]++;
                }
             }
             m = get_compress_suff_link(m, bohr);
        }
    }
    for (int i = 0; i <100000; i++){
        if (C[i] == substr_vec.size()) 
            cout << i << endl;
    }
}