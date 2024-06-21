#ifndef FUNCTION_DECLARE
#define FUNCTION_DECLARE
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "struct_define.h"
using namespace std;
/**
 * @brief 读取transaction数据并转为TransactionSet格式
 *
 * @param file 文件路径
 * @return TransactionSet( vector<Transaction> )
 */
TransactionSet read_file(string const &file);

/**
 * @brief 遍历TransactionSet，统计每个item出现的次数
 *
 * @param transactions transactionSet
 * @return ItemFreq( map<string, int> )
 */
ItemFreq get_frequent_item(TransactionSet const &transactions);

/**
 * @brief 删除不频繁的项
 *
 * @param result 需要删除的集合
 * @param threshold 低于此次数的项将被删去
 */
void delete_unfrequent_item(
    ItemFreq &result,
    int threshold);

/**
 * @brief 删除非频繁项后进行排序
 *
 * @param result ( map<string, int> )
 * @param transactions transactionSet
 * @return FPset
 */
FPset re_rank_frequent_item(
    ItemFreq const &result,
    TransactionSet &transactions);

/**
 * @brief 生成HeaderTable
 *
 * @param result ( map<string, int> )
 * @param threshold 阈值
 * @return HeaderTable
 */
HeaderTable make_header_table(
    ItemFreq const &result,
    int const &threshold);

/**
 * @brief 查找item是否在headerTable中，若存在，返回其索引；不存在，返回NON_EXIST
 *
 * @param item string 待查找项目
 * @param headerTable 查找项头范围
 * @return int
 */
int pos_in_header_table(
    string item,
    HeaderTable const &headerTable);

/**
 * @brief 查找node是否在children中，若存在，返回其索引；不存在，返回NON_EXIST
 *
 * @param node 待查找结点
 * @param children 查找子结点范围
 * @return int
 */
int pos_in_children(
    FPnode node,
    Children const &children);

/**
 * @brief 生成FPtree
 *
 * @param freqNodes 事务记录结点集
 * @param headerTable 项头表
 * @return FPnode*
 */
FPnode *create_FP_tree(
    FPset const &freqNodes,
    HeaderTable &headerTable);

/**
 * @brief 挖掘一条FP子树
 *
 * @param headerItem 子树叶子结点（记录在HeaderItem.phead中）
 * @param threshold 阈值
 * @return vector< pair< string, unsigned int > > 升序排列的(项目,频次)对
 */
vector< pair< string, unsigned int > > mine_FP_node(
    HeaderItem const &headerItem,
    int threshold);

/**
 * @brief 深度优先遍历获取幂集
 *
 * @param input 待生成幂集的原集合
 * @param result 储存幂集的集合
 * @param current 暂存的一次子集
 * @param index 记录第几个元素，判定结束条件
 */
void gen_subsets(
    vector< pair< string, unsigned int > > const &input,
    vector< pair< vector< string >, unsigned int > > &result,
    vector< string > &current,
    int index);

/**
 * @brief 整合上述函数（gen_subsets）
 *
 * @param input 待生成幂集的原集合
 * @return vector< pair< vector< string >, unsigned int > >
 */
vector< pair< vector< string >, unsigned int > > get_subsets(
    vector< pair< string, unsigned int > > const &input);

/**
 * @brief DFS生成DOT文件
 *
 * @param node 当前结点
 * @param file 输出文件
 */
void gen_dot_file_DFS(
    FPnode *node,
    ofstream &file);

/**
 * @brief 生成DOT文件
 *
 * @param root FPtree根结点
 * @param filename 输出文件
 */
void gen_dot_file(
    FPnode *root,
    string const &filename);

#endif