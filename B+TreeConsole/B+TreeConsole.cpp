// RedBlackTreeConsole.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <random>
#include "DataSearchTree.h"

#define ORDER 12
//#define TREE_ORDER 9
//#define DATA_ORDER 18

#define INSERT_COUNT 710
#define REMOVE_COUNT 538

#define ROUND 42

using namespace std;
using namespace DataSearchTree;

int main()
{
	const int insertCount = INSERT_COUNT;
	const int removeCount = REMOVE_COUNT;

	const int round = ROUND;

	bool showEachHealthStatus = true;

	bool showInsertDetails = true;
	bool showRemoveDetails = true;

	bool healthHeavyCheck = true;

	random_device dev;
	mt19937 rng(dev());
	uniform_int_distribution<std::mt19937::result_type> insertRandom(0, 9999); // distribution in range [1, 6]

	random_device dev2;
	mt19937 rng2(dev2());
	uniform_int_distribution<std::mt19937::result_type> removeIndexRandom(0, insertCount - 1); // distribution in range [1, 6]

	BpTree tree = { ORDER };
	//BpTree tree = { TREE_ORDER, DATA_ORDER };

	bool totHealth = true;

	for (int r = 0; r < round; r++) {
		cout << "ORDER : { TREE: " << to_string(tree.GetTreeOrder()) << ", DATA: " << to_string(tree.GetDataOrder()) << " }\n";
		cout << "----------------------------------------------------------------------------------------------" << "\n";
		cout << "ROUND : " << r + 1 << "\n";

		int insertArr[insertCount];
		int removeIndexArr[removeCount];
		bool isHealth = false;

		//random insert array
		for (int i = 0; i < insertCount; i++)
			insertArr[i] = insertRandom(rng);

		isHealth = tree.HealthCheck();

		//insert into tree
		if (insertCount > 0) {
			if (showInsertDetails) cout << "Inserted : { ";
			tree.Insert(insertArr[0], "DATA" + to_string(insertArr[0]));
			if (healthHeavyCheck) isHealth = tree.HealthCheck();
			if (showInsertDetails) cout << to_string(insertArr[0]);
			for (int i = 1; i < insertCount; i++) {
				tree.Insert(insertArr[i], "DATA" + to_string(insertArr[i]));
				if (healthHeavyCheck) isHealth = isHealth && tree.HealthCheck();
				if (showInsertDetails) cout << ", " << to_string(insertArr[i]);
			}
			if (showInsertDetails) cout << " }";
		}
		if (showInsertDetails) cout << " \n";

		isHealth = isHealth && tree.HealthCheck();

		for (int i = 0; i < insertCount; i++)
			isHealth = isHealth && tree.SearchElement(insertArr[i]) && tree.SearchElement(insertArr[i])->GetKey() == insertArr[i];
		totHealth = totHealth && isHealth;

		//Check bugs
		if (showEachHealthStatus) cout << "Inserted Tree Health : " << (isHealth ? "OK, no BUG!" : "Warning! Error occured!") << "\n";
		if (showEachHealthStatus) cout << " \n";

		//random remove array
		for (int i = 0; i < removeCount; i++)
			removeIndexArr[i] = removeIndexRandom(rng2);

		isHealth = tree.HealthCheck();

		//remove from tree
		if (removeCount > 0) {
			if (showRemoveDetails) cout << "Removed : { ";
			tree.Remove(insertArr[removeIndexArr[0]]);
			if (healthHeavyCheck) isHealth = tree.HealthCheck();
			if (showRemoveDetails) cout << to_string(insertArr[removeIndexArr[0]]);
			for (int i = 1; i < removeCount; i++) {
				tree.Remove(insertArr[removeIndexArr[i]]);
				if (healthHeavyCheck) isHealth = isHealth && tree.HealthCheck();
				if (showRemoveDetails) cout << ", " << to_string(insertArr[removeIndexArr[i]]);
			}
			if (showRemoveDetails) cout << " }";
		}
		if (showRemoveDetails) cout << " \n";

		isHealth = isHealth && tree.HealthCheck();

		for (int i = 0; i < removeCount; i++)
			isHealth = isHealth && !tree.SearchElement(insertArr[removeIndexArr[i]]);
		totHealth = totHealth && isHealth;

		//Check bugs
		if (showEachHealthStatus) cout << "Removed Tree Health : " << (isHealth ? "OK, no BUG!" : "Warning! Error occured!") << "\n";
		if (showEachHealthStatus) cout << " \n";

		//Recheck
		for (int i = 0; i < removeCount; i++) {
			int removedKey = insertArr[removeIndexArr[i]];
			for (int j = 0; j < insertCount; j++)
				if (removedKey == insertArr[j])
					insertArr[j] = INT_MIN;
		}

		for (int i = 0; i < insertCount; i++) {
			if (insertArr[i] != INT_MIN)
				isHealth = isHealth && tree.SearchElement(insertArr[i]) && tree.SearchElement(insertArr[i])->GetKey() == insertArr[i];
		}

		totHealth = totHealth && isHealth;

		//Check bugs
		if (showEachHealthStatus) cout << "Recheck Tree Health : " << (isHealth ? "OK, no BUG!" : "Warning! Error occured!") << "\n";
		if (showEachHealthStatus) cout << " \n";
	}

	tree.Clear();

	cout << " \n";
	cout << "All Tree Health : " << (totHealth ? "OK, no BUG!" : "Warning! Error occured!") << "\n";
	cout << " \n";

	system("pause");
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
