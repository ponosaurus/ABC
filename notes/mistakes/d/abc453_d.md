## 1. 再帰が深すぎる
再帰BFSでTLEとMLEを出した

## 2. bad_alloc
queueからStateを取り出したタイミングでvisitedをtrueにしていた
-> 他の経路で同じマスに辿り着いて、同じ経路をpushしてしまうミスが起こる！
pushしたら即座にvisitedに記録しなきゃいけない

## 3. visitedを使った経路の逆再生はできない
visitedを使ってcurrent.prev_dirから経路の逆再生を試みたが、「そのマスに到達しうるルート」をただ単に漁っていけばstartに着けるわけではない
関係ない方向に進んで無限ループに陥りうる
新たにpre_state[x][y][dir] = {prev_x, prev_y, prev_prev_dir}を保存する必要があった
currentから一つ前のStateに遡れる必要があった

## 4. structのデフォルトコンストラクタがない
struct State を作った時に、`State(int x, int y, int prev_dir) : x(x), y(y), prev_dir(prev_dir) {}` で引数ありのコンストラクタを自作したため、引数なしのデフォルトコンストラクタを失ってしまった
Stateの配列を作るために、デフォルトコンストラクタ `State() {}` を書き足した