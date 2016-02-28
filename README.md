# CodersStrikeBack
codingame

# Rule
The players each control a team of two pods during a race. As soon as a pod completes the race, that pod's team is declared the winner.
書くプレイヤーはレース中に2つのポッドを操作します。 ２つのポッドがレースをゴールしたチームが勝者です。

The circuit of the race is made up of checkpoints. To complete one lap, your pod must pass through each one in order and back through the start. 
レースの周回にはチェックポイントが設置されています。 1つの周回で各ポットは昇順にポイントを回ってスタート地点に戻ります。

The first player to reach the start on the final lap wins.
最後の周回はスタート地点に到達した人の勝利です

The game is played on a map 16000 units wide and 9000 units high. The coordinate X=0, Y=0 is the top left pixel.
このゲームでは横幅16000、高さ9000のフィールドで行われます。一番左上の座標が(0, 0)となります。

The checkpoints work as follows:
チェックポイントは以下のとおりです

* The checkpoints are circular, with a radius of 600 units.
チェックポイントは円で、半径が600です

Checkpoints are numbered from 0 to N where 0 is the start and N-1 is the last checkpoint.
* チェックポイントは0 - Nでナンバリングされており、0がスタート、N-1が最後のチェックポイントです。

The disposition of the checkpoints is selected randomly for each race.
各レースにおいてチェックポイントはランダムに選択されます。


The pods work as follows:
ポッドの動作について

* The pods have a circular force-field around their center, with a radius of 400 units, which activates in case of collisions with other pods.
ポッドは円の半径400以内に到達した場合に他のポッドとの衝突判定が発生します。

* To pass a checkpoint, the center of a pod must be inside the radius of the checkpoint.
チェックポイントを通過するためには、ポッドはチェックポイントの円の中に入らないければいけません。

* To move a pod, you must print a target destination point followed by a thrust value. Details of the protocol can be found further down.
ポッドを動かすためには目標の座標を出力してください。細かい紹介については下のページの説明に記述します。

* The thrust value of a pod is it's acceleration and must be between 0 and 200.
ポッドに指定できる加速度は0 - 200の間で指定できます。

* The pod will pivot to face the destination point by a maximum of 18 degrees per turn and will then accelerate in that direction.
1回の行動で最大18度の旋回が可能です

* If none of your pods make it to their next checkpoint in under 100 turns, you are eliminated and lose the game.
もし100ターン以内に次のチェックポイントに到達出来なかったポッドはゲームから取り除かれます。

* The pods may move normally outside the game area.
ポッドは場外に飛び出しても動作します。

* 特別ルール
You may activate the pod's shields by printing the SHIELD keyword instead of accelerating. 
あなたは移動する代わりにSHIELDと出力することでシールドを発動されることができます。
This will give the pod much more weight if it collides with another. 
この動作を行うことで衝突時のポッドが重くなります。
However, the pod will not be able to accelerate for the next 3 turns.
しかし、このコマンド後3ターンは行動ができません。


ゲームの入力

初期化

laps : ゲーム全体のラップ
checkpointCount : チェックポイントの数と座標

ターンごとの入力

