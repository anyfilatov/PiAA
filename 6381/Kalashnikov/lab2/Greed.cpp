#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct node 
{
    char ch;
    bool visited;

    node(char ch)
    : ch(ch)
    , visited(0)
    {}

    friend std::ostream & operator << (std::ostream & os, const node & a)
    {
        cout << a.ch << "(" << a.visited << ")";
	    return os;
    }
};

struct edge
{
    struct node *from, *to;
    float length;

    edge()
    : from(NULL)
    , to(NULL)
    , length(-1)
    {

    }

    edge(node *first, node *second, float length)
    : from(first)
    , to(second)
    , length(length)
    {
        //cout << "Created: " << *this <<  endl;
    }

    friend std::ostream & operator << (std::ostream & os, const edge & a)
    {
        cout << a.from->ch << " : " << a.length << " : " << a.to->ch << endl;
	    return os;
    }

/*     bool operator==(const edge& a) const
    {
        return this->to.ch == a.to.ch  && this->max.ch == a.max.ch  && this->from.ch == a.from.ch;
    } */
        
};

class PathFinder
{
    public:
    struct node *start, *finish, *current;
    vector <struct edge> v_edges;
    vector <struct node *> v_nodes;
    string path;

    PathFinder()
    : path()
    {
        char first, second;
        float length;
        
        cin >> first >> second;

        v_nodes.push_back(new node(first));
        v_nodes.push_back(new node(second));

        start  = v_nodes[0];
        finish = v_nodes[1];
        current = start;
        current->visited = true;
        path += first;

        while(cin >> first >> second >> length)
        {
            bool add_first = true, add_second = true;
            
            for(auto i : v_nodes)
            {
                if(i->ch == first) add_first  = false;
                else if(i->ch == second) add_second = false;
            }

            if(add_first)
            {
                //cout << "Add new!" << endl;
                v_nodes.push_back(new node(first));
            }
            if(add_second)
            {
                //cout << "Add new!" << endl;
                v_nodes.push_back(new node(second));
            }

            struct edge temp(find_node(first), find_node(second), length);

            v_edges.push_back(edge(find_node(first), find_node(second), length));
        }
    }

    struct node *find_node(char ch)
    {
        for(int i = 0; i < v_nodes.size(); i++)
        {
            if(v_nodes[i]->ch == ch) return v_nodes[i];
        }
        
        //cout << "Not found!" << endl;
        return NULL;

    }

    void print_info()
    {
        for(auto i : v_nodes)
        {
            cout << *i << endl;
        }

        for(auto i : v_edges)
        {
            cout << i << endl;
        }
    }

    void makestep()
    {
        struct edge min_edge;

        for(auto i : v_edges)
        {
            if(i.from == current && i.to->visited == false && ( min_edge.length == -1 || min_edge.length > i.length) )
                min_edge = i;
        } 

        if (min_edge.to == NULL)
        {
            path.pop_back();
            current = find_node(path.back());
        }
        else
        {
            current = min_edge.to;
            current->visited = true;
            path += current->ch;  
        }
        //cout << min_edge << endl;
        
    }

    void find_path()
    {
        while(current != finish && !path.empty())
        {
            makestep();
        }

        cout << path << endl;  
    }
};

int main() 
{
    PathFinder a;
    a.find_path();

    return 0;
}