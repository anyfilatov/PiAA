#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

struct node 
{
    char ch;
    bool visited;
    struct node *camefrom;
    float cost;

    node(char ch)
    : ch(ch)
    , visited(0)
    , camefrom(NULL)
    , cost(-1)
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

    bool operator==(const edge& a) const
    {
        return to->ch == a.to->ch && from->ch == a.from->ch;
    } 
        
};

class PathFinder
{
    public:
    struct node *start, *finish, *current;
    vector <struct edge> v_edges;
    vector <struct node *> v_nodes;
    list <struct edge> l_ways_to_go;
    string path;
    bool dead_end;

    PathFinder()
    : path()
    , dead_end(false)
    {
        char first, second;
        float length;
        
        cin >> first >> second;

        v_nodes.push_back(current = start  = new node(first));
        v_nodes.push_back(finish = new node(second));

        current->visited = true;

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
        cout << "Nodes: " << endl;
        for(auto i : v_nodes)
        {
            cout << *i << endl;
        }

        cout << "Edges: " << endl;
        for(auto i : v_edges)
        {
            cout << i << endl;
        }

        cout << "Ways to go: " << endl;
        for(auto i : l_ways_to_go)
        {
            cout << i << endl;
        }
    }

    float heuristic_func(char ch)
    {
        return abs(finish->ch - ch);
    }

    void makestep()
    {
        struct edge min_edge;

        for(auto i : l_ways_to_go)
        {
            if( min_edge.length == -1
            || i.to->cost + heuristic_func(i.to->ch) <= min_edge.to->cost + heuristic_func(min_edge.to->ch)
            && i.to->ch > min_edge.to->ch )
                min_edge = i;
        }

        if (min_edge.to == NULL)
        {
            //cout << "Finish is unreachable!" << endl;
            dead_end = true;
        }
        else
        {
            l_ways_to_go.remove(min_edge);
            current = min_edge.to;
            current->visited = true;
        }
        //cout << min_edge << endl;
    }

    void update_ways_to_go()
    {
       // cout << "Updating ways: " << endl;
        for(auto i : v_edges)
        {
            if(i.from == current && (i.from->cost + i.length <= i.to->cost || i.to->cost == -1) )
            {
                i.to->cost = i.from->cost + i.length;
                i.to->camefrom = i.from;
                l_ways_to_go.push_back(i);
            }
        } 

        //print_info();
    }

    void print_path()
    {
        do
        {
            path = current->ch + path;
            current = current->camefrom;
        }while(current != NULL);

        cout << path << endl;
    }

    void find_path()
    {
        while(current != finish && !dead_end)
        {
            update_ways_to_go();
            makestep();
        }

        //cout << "Done!" << endl;
        print_path();
    }
};

int main() 
{
    PathFinder a;
    a.find_path();

    return 0;
}