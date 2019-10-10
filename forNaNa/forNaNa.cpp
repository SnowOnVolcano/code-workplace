#include "hashtable.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cstring>
using namespace std;

/*
	基本思路是通过对两个最小化DFA进行同步DFS遍历，
	而遍历的原则是：
		对于两者访问的下一个节点：
		1.都未被访问，则访问，进入深一层的遍历；
		2.都已访问，改变字符输入寻求下一个未被访问的节点。

	需要注意的是，当两者在遍历的过程中，碰到以下情况时：
	1.其中一个到达了结束状态而另一个还处于非结束状态；
	2.两者即将访问的下一个节点的深度不一样（都没被访问的话视作深度相同）;
	则认为两个最小化DFA不是等价的。
	如果两者成功同步访问完所有的节点，即遍历过程中没有碰到以上的两种情况，则两个最小化DFA是等价的。
*/

int P, Q;									//两个最小化DFA的状态数
int MIN1[ 1600 ][ 5 ], MIN2[ 1600 ][ 5 ];	//两个最小化DFA的状态转移表
int start1, start2;							//两个最小化DFA的开始状态
vector<int> fin1, fin2;						//两个最小化DFA的结束状态
int deep;									//同步遍历时的深度
int vis1[ 1600 ], vis2[ 1600 ];				//两个最小化DFA在同步遍历时各个状态的深度

bool traversal( int n1, int n2 ) {
	int i, m1, m2;
	vis1[ n1 ] = vis2[ n2 ] = ++deep;
	for ( i = 0; i < 4; i++ ) {
		m1 = MIN1[ n1 ][ i ];
		m2 = MIN2[ n2 ][ i ];
		if ( ( find( fin1.begin(), fin1.end(), m1 ) == fin1.end() && find( fin2.begin(), fin2.end(), m2 ) != fin2.end() )
			|| ( find( fin1.begin(), fin1.end(), m1 ) != fin1.end() && find( fin2.begin(), fin2.end(), m2 ) == fin2.end() ) )
			break;
		if ( vis1[ m1 ] != vis2[ m2 ] )
			break;
		if ( vis1[ m1 ] == 0 && vis2[ m2 ] == 0 ) {
			if ( !traversal( m1, m2 ) )
				break;
		}
	}
	--deep;
	return i == 4;
}

bool equal() {
	if ( P != Q )
		return false;
	memset( vis1, 0, sizeof( vis1 ) );
	memset( vis2, 0, sizeof( vis2 ) );
	deep = 0;
	return traversal( start1, start2 );
}


int main()
{
    P = Q =3;
    MIN1[0][0] = 3;
    MIN1[0][1] = 2;    
    MIN1[1][0] = 2;
    MIN1[1][1] = 2;

    MIN2[0][0] = 1;
    MIN2[0][1] = 2;    
    MIN2[1][0] = 2;
    MIN2[1][1] = 2;
    start1 = start2 = 0;

    fin1.push_back(3);
    fin1.push_back(2);
    fin2.push_back(1);
    fin2.push_back(3);
	cout << ( equal() ? "YES": "NO" ) << endl << endl << endl;
	return 0;
}