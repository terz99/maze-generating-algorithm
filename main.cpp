#include <bits/stdc++.h>
using namespace std;

const int MAX_LEN = 2500;
bool VIS[MAX_LEN][MAX_LEN];
int old_wid, old_len, LEN, WID;
char mat[MAX_LEN][MAX_LEN];
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};
int ddx[] = {0, 0, 2, -2};
int ddy[] = {2, -2, 0, 0};

class union_find{

    const static int MAX_NODES = 1000;
    int parent[MAX_NODES];
    int size[MAX_NODES];

public:

    union_find(int s = MAX_NODES){
        for(int i = 0; i < s; i++){
            parent[i] = i;
            size[i] = 1;
        }
    }

    bool connected(int u, int v){
        return find(u) == find(v);
    }

    int find(int u){
        while(parent[u] != u){
            parent[u] = parent[parent[u]];
            u = parent[u];
        }
        return u;
    }

    void connect(int u, int v){

        int u_root = find(u);
        int v_root = find(v);

        if(size[u_root] > size[v_root]){
            parent[v_root] = u_root;
        } else if(size[u_root] < size[v_root]){
            parent[u_root] = v_root;
        } else {
            parent[u_root] = v_root;
            size[v_root]++;
        }
    }
};

class state{

public:

    int x, y;
    vector< pair<int,int> > backtrack;

    state(){

    }

    state(int x, int y, vector< pair<int,int> > backtrack){
        this->x = x;
        this->y = y;
        this->backtrack = backtrack;
    }

};

int get_random_index(int limit){
    return (int)(rand()%limit);
}

pair<int, int> get_coord(int x){
    return make_pair( 2*(x/old_wid)+1, 2*(x%old_wid)+1 );
}

void search_path(int s, int e){

    pair<int, int> coord = get_coord(s);
    pair<int, int> fcoord = get_coord(e);
    // cout << s << " : " << coord.first << " " << coord.second << endl;
    // cout << e << " : " << fcoord.first << " " << fcoord.second << endl;

    for(int i = 0; i < 4; i++){

        int nx = coord.first + ddx[i];
        int ny = coord.second + ddy[i];

        if(nx == fcoord.first && ny == fcoord.second){
            mat[coord.first+dx[i]][coord.second+dy[i]] = (char)32;
            break;
        }

    }
}

void draw_maze(int num_nodes, vector<int> maze[], int len, int wid) {

    len = 2*len+1;
    wid = 2*wid+1;
    LEN = len;
    WID = wid;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < wid; j++){
            mat[i][j] = '#';
        }
    }

    // setting entrance and exit
    mat[0][1] = (char)32;
    mat[len-1][wid-2] = (char)32;

    for(int i = 1; i < len; i += 2){
        for(int j = 1; j < wid; j += 2){
            mat[i][j] = (char)32;
        }
    }

    for(int i = 0; i < num_nodes; i++){
        for(int j = 0; j < maze[i].size(); j++){
            search_path(i, maze[i][j]);
        }
    }

    // char cnt = '0';
    // for(int i = 1; i < len; i += 2){
    //     for(int j = 1; j < wid; j += 2){
    //         mat[i][j] = cnt;
    //         cnt++;
    //     }
    // }

    for(int i = 0; i < len; i++){
        for(int j = 0; j < wid; j++){
            cout << mat[i][j];
        }
        cout << "\n";
    }

}
void generate_maze(int num_nodes, vector< pair<int, int> > &adj, vector<int> maze[]){

    bool vis[adj.size()+5];
    memset(vis, false, sizeof(vis));

    union_find uf(num_nodes);
    int tree_size = 1;
    // cout << num_nodes << "\n";
    while(tree_size < num_nodes){

        int rand_idx = get_random_index((int)adj.size());
        while(vis[rand_idx]){
            rand_idx = get_random_index((int)adj.size());
        }
        vis[rand_idx] = true;

        int u = adj[rand_idx].first;
        int v = adj[rand_idx].second;
        // cout << u << " " << v << "\n";
        if(!uf.connected(u, v)){
            // cout << u << " " << v << "\n";
            uf.connect(u, v);
            maze[u].push_back(v);
            maze[v].push_back(u);
            tree_size++;
        }
    }
}

int main() {

    srand(time(NULL));
    ios_base::sync_with_stdio(false);

    int len, wid;
    cout << "Enter the length and the width";
    cout << " (if you type -1 for the width then\nthe grid will be considered as a square)\n";
    cin >> len >> wid;
    if(wid == -1){
        wid = len;
    }
    old_wid = wid;
    old_len = len;

    int num_nodes = len*wid;
    vector< pair<int, int> > adj;

    int idx = 0;
    for(int i = 0; i < len; i++){
        for(int j = 0; j < wid; j++){
            if(j < wid-1){
                adj.push_back( make_pair(idx, i*wid+j+1) );
            }
            if(i < len-1){
                adj.push_back( make_pair(idx, (i+1)*wid+j) );
            }
            idx++;
        }
    }


    vector<int> maze[num_nodes+5];

    generate_maze(num_nodes, adj, maze);
    draw_maze(num_nodes, maze, len, wid);

    return 0;
}
