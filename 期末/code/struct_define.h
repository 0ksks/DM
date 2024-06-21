#ifndef STRUCT_DEFINE
#define STRUCT_DEFINE

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * @brief 频繁模式树(Frequent Pattern Tree)的结点
 *
 */
struct FPnode {
    string itemName;                             // 项目名称
    unsigned int frequency;                      // 项目频次
    FPnode *parent;                              // 父结点
    vector< FPnode * > children;                 // 子结点
    bool operator<(FPnode const &other) const {  // 频次比较器
        return frequency < other.frequency;
    }
    /**
     * @brief 无参数构造函数
     *
     */
    FPnode() {
        itemName = "";
        frequency = 0;
        parent = nullptr;
        children.clear();
    }
};

/**
 * @brief 项头表(Header Table)中的每一项
 *
 */
struct HeaderItem {
    string itemName;                                 // 项目名称
    unsigned int frequency;                          // 项目频次
    list< FPnode * > phead;                          // 链表表头
    bool operator<(HeaderItem const &other) const {  // 频次比较器
        return frequency < other.frequency;
    }
};

/**
 * @brief 事务集中的每一条事务
 *
 */
struct Transaction {
    string tranID;            // 事务ID
    unsigned int itemAmount;  // 项目个数
    list< FPnode > itemList;  // 储存项目的列表
};

/**
 * @brief 频繁模式
 *
 */
struct FrequentMode {
    vector< string > mode;  // 频繁模式
    unsigned int frequncy;  // 模式频次
};

typedef vector< HeaderItem > HeaderTable;  // 项头表

typedef vector< Transaction > TransactionSet;  // 事务集

typedef vector< vector< FPnode > > FPset;  // 频繁模式集

typedef vector< FPnode * > Children;  // 孩子结点表

typedef list< FPnode > ItemList;  // 储存项目的列表

typedef list< FPnode * > ItemPtrList;  // 项头表中的链表

typedef map< string, unsigned int > ItemFreq;  // 项目频次对照表

#define NON_EXIST -100  // 不存在索引

#endif