#include "function_declare.h"

int main() {
    float SUPPORT_RATE = 0.05;   // 设置支持率
    string INPUT_TRANSACTION =  // 设置事务集文件
        "/Users/admin/Desktop/LEARN/大三下/DM/project/data/testTrans.txt";
    string OUTPUT_FP_TREE =  // 设置FP树打印文件
        "/Users/admin/Desktop/LEARN/大三下/DM/project/data/testTree.dot";

    TransactionSet transactions = read_file(INPUT_TRANSACTION);  // 读取文件并生成TransactionSet

    int transactionNum = transactions.size();            // 事务条数
    int threshold = int(SUPPORT_RATE * transactionNum);  // 计算最小支持度
    printf("total:%d|support rate:%.3f|threshold:%d\n", transactionNum, SUPPORT_RATE, threshold);
    // 打印信息

    ItemFreq result = get_frequent_item(transactions);  // 从事务集中计算项目频次

    delete_unfrequent_item(  // 删除低于阈值的项目
        result,
        threshold);

    FPset reRankResult = re_rank_frequent_item(  // 对删除低于阈值项目后的项目集排序
        result,
        transactions);

    HeaderTable headerTable = make_header_table(  // 初始化项头表
        result,
        threshold);

    FPnode* FPtreeRoot = create_FP_tree(  // 构建FP树，同时构建项头表
        reRankResult,
        headerTable);

    for (
        HeaderTable::const_iterator iter_headerItem = headerTable.cbegin();
        iter_headerItem != headerTable.cend();
        iter_headerItem++) {
        HeaderItem headerItem = *iter_headerItem;
        vector< pair< string, unsigned int > > freqPattern = mine_FP_node(
            headerItem,
            threshold);
        vector< pair< vector< string >, unsigned int > > subsets = get_subsets(freqPattern);
        printf("%d { %s }\n", headerItem.frequency, headerItem.itemName.c_str());
        for (
            vector< pair< vector< string >, unsigned int > >::const_iterator iter_pair = subsets.cbegin();
            iter_pair != subsets.cend();
            iter_pair++) {
            cout << iter_pair->second;
            cout << " { " << headerItem.itemName << " ";
            for (
                vector< string >::const_iterator iter_str = iter_pair->first.cbegin();
                iter_str != iter_pair->first.cend();
                iter_str++) {
                cout << *iter_str << " ";
            }
            cout << "}\n";
        }
    }
    gen_dot_file(  // 生成FP树的可视化文件DOT
        FPtreeRoot,
        OUTPUT_FP_TREE);
    return 0;
}