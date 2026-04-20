/**
 * @file
 * ABC453_D Go Straight
 * https://atcoder.jp/contests/abc453/tasks/abc453_d
 * 
 * 色々いじってAC
 * 
 * 
 * @brief
 * grid迷路 再帰
 *
 * @note
 * Problem Statement:
 *  迷路問題 SからGまでのパスを出力
 *  `#`: 出入りできない (0方向)
 *  `.`: 自由に出入りできる (4方向)
 *  `o`: 直前の移動方向にしか移動できない (1方向)
 *  `x`: 直前の移動方向以外にしか移動できない (3方向)
 *  `S`: スタート地点
 *  `G`: ゴール地点
 * 
 * Constraints:
 *  1 <= H, W <= 1000
 * 
 * Input:
 *  H W
 *  グラフの情報 (H行W列)
 * 
 * Output:
 *  Yes / No
 *  SからGまでの移動例 (U, D, L, R)
 * 
 * @todo
 *
 */

// include
#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
#include <boost/dynamic_bitset.hpp>
using namespace boost;

// define
#define fore(x, a) for (auto &x : a)
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define repp(i, m, n) for (int i = (int)(m); i < (int)(n); i++)
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()

// typedef
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef vector<vvb> vvvb;
typedef vector<string> vs;

const double pi = 3.141592653589793238;
const int inf = 1073741823;
const ll infl = 1LL << 60;
const int mod = 998244353;
struct Init { Init() { ios::sync_with_stdio(0); cin.tie(0); } }init;

int h, w;

/**
 * @brief 
 * グラフの情報
 * `#`: 出入りできない (0方向)
 * `.`: 自由に出入りできる (4方向)
 * `o`: 直前の移動方向にしか移動できない (1方向)
 * `x`: 直前の移動方向以外にしか移動できない (3方向)
 * `S`: スタート地点
 * `G`: ゴール地点
 */
vs grid;

/**
 * @brief 
 * grid中の各マスについて、状態ごとに既に通ったかどうかを確認する
 * 例えばvisited[x][y][prev_dir] = true なら、Stateが(x, y, prev_dir)である状態は既に通ったことがある
 */
bool visited[1000][1000][4];

/// @brief 現在の状態を表す構造体 座標と直前の移動方向を持つ
struct State {
  int x;
  int y;

  /**
   * @brief
   * 直前の移動方向
   * 初期状態では適当に0とでもしておく
   * 0: 下 (x+=1)
   * 1: 上 (x-=1)
   * 2: 右 (y+=1)
   * 3: 左 (y-=1)
   */
  int prev_dir;

  State() {}
  State(int x, int y, int prev_dir) : x(x), y(y), prev_dir(prev_dir) {}

  /**
   * @brief 現在の状態からdir方向に移動できるかどうかを返す
   * @param dir 移動方向
   */
  bool is_movable(int dir) {
    /// 範囲外に出ないか
    if(dir == 0 && x == h-1) return false;
    if(dir == 1 && x == 0) return false;
    if(dir == 2 && y == w-1) return false;
    if(dir == 3 && y == 0) return false;

    /// 来訪済みでないか
    int next_x = x + (dir == 0) - (dir == 1);
    int next_y = y + (dir == 2) - (dir == 3);
    if(visited[next_x][next_y][dir]) return false;

    char cell = grid[x][y];
    if(cell == '#') return false;
    if(cell == '.') return true;
    if(cell == 'o') return dir == prev_dir;
    if(cell == 'x') return dir != prev_dir;
    return true; // SやGのマスは自由に出入りできる
  }
};

/**
 * @brief
 * 移動方向を数字で扱いたいので、出力用にint -> charのマッピングを用意しておく
 * dir_to_char[0] = 'D' のように使う
 */
const string dir_to_char = "DURL";

/// @brief BFSで使う、探索すべきStateを保持するqueue
queue<State> bfs;

/** 
 * @brief 
 * 各Stateについて、どのStateから来たのかを保存するための配列
 * prev_state[x][y][prev_dir]で指定する
 */
State prev_state[1000][1000][4];

int main() {
  cin >> h >> w;
  grid.resize(h);

  State start(0, 0, 0);
  rep(i, h) {
    cin >> grid[i];
    rep(j, w) {
      if(grid[i][j] == 'S') {
        start = State(i, j, 0);
      }
    }
  }

  string moves = "";
  bfs.push(start);
  State current(0, 0, 0); /// 初期状態のcurrentはあり得ない値にしておく

  while(!bfs.empty()) {
    current = bfs.front();
    bfs.pop();

    if(grid[current.x][current.y] == 'G') {
      moves += dir_to_char[current.prev_dir];
      break;
    }

    rep(i, 4) {
      if(current.is_movable(i)) {
        int next_x = current.x + (i==0) - (i==1);
        int next_y = current.y + (i==2) - (i==3);
        bfs.push(State(next_x, next_y, i));
        visited[next_x][next_y][i] = true;
        prev_state[next_x][next_y][i] = current;
      }
    }
  }

  if(moves == "") {
    cout << "No" << endl;
    return 0;
  }

  /**
   * 手順を巻き戻してmovesに記録していく
   */
  while(true) {
    current = prev_state[current.x][current.y][current.prev_dir];
    if(current.x == start.x && current.y == start.y) break;
    moves += dir_to_char[current.prev_dir];
  }

  reverse(all(moves));
  cout << "Yes" << endl;
  cout << moves << endl;

  return 0;
}
