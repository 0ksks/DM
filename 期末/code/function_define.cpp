#include "function_declare.h"

TransactionSet read_file(string const &file) {
    TransactionSet transactions;
    ifstream fin(file);
    if (fin.fail()) {  // 如果文件打开失败
        cout << "ERROR: FILE OPEN FAILED\n";
        exit(-1);
    }
    string rowContent;
    while (getline(fin, rowContent)) {  // 读取每一行的内容
        istringstream row(rowContent);
        Transaction tmp_transaction;
        row >> tmp_transaction.tranID;      // 字段：ID
        row >> tmp_transaction.itemAmount;  // 字段：物品个数
        for (unsigned int i = 0; i < tmp_transaction.itemAmount; i++) {
            FPnode tmp_node;                               // 创建FPtree的一个结点
            row >> tmp_node.itemName;                      // 字段：物品
            tmp_transaction.itemList.push_back(tmp_node);  // 向一条transaction的itemList中添加一个结点
        }
        transactions.push_back(tmp_transaction);  // 向transactionSet中添加一条记录
    }
    return transactions;
}

ItemFreq get_frequent_item(TransactionSet const &transactions) {
    ItemFreq result;
    for (  // 遍历事务集中的每一条事务
        TransactionSet::const_iterator iter_transaction = transactions.cbegin();
        iter_transaction != transactions.cend();
        iter_transaction++) {
        for (  // 遍历事务中的每一个项目
            ItemList::const_iterator iter_item = iter_transaction->itemList.cbegin();
            iter_item != iter_transaction->itemList.cend();
            iter_item++) {
            result[iter_item->itemName]++;  // 记录累计频次
        }
    }
    return result;
}

/**
 * @brief 用来给ItemFreq排序的比较器
 */
bool ItemFreq_less_than(
    const pair< string, unsigned int > &a,
    const pair< string, unsigned int > &b) {
    return a.second > b.second;
}

void delete_unfrequent_item(
    ItemFreq &result,
    int threshold) {
    vector< string > deleteKeys;
    for (
        ItemFreq::const_iterator iter_itemFreq = result.cbegin();
        iter_itemFreq != result.cend();
        iter_itemFreq++) {
        if (iter_itemFreq->second < threshold) {
            deleteKeys.push_back(iter_itemFreq->first);  // 记录不频繁的项
        }
    }
    for (
        vector< string >::const_iterator iter_string = deleteKeys.cbegin();
        iter_string != deleteKeys.cend();
        iter_string++) {
        result.erase(*iter_string);  // 删除不频繁的项
    }
}

/**
 * @brief 用来给频繁项排序的比较器
 */
bool FPnode_greater_than(
    const FPnode &a,
    const FPnode &b) {
    return (a.frequency > b.frequency);
}

FPset re_rank_frequent_item(
    ItemFreq const &result,
    TransactionSet &transactions) {
    FPset reRankResult;
    for (
        TransactionSet::iterator iter_transaction = transactions.begin();
        iter_transaction != transactions.end();
        iter_transaction++) {
        ItemList::iterator iter_item = iter_transaction->itemList.begin();
        while (iter_item != iter_transaction->itemList.end()) {
            if (result.find(iter_item->itemName) == result.end()) {  // 若该项非频繁，删除
                iter_item = iter_transaction->itemList.erase(iter_item);
            } else {  // 若频繁，修改频率
                iter_item->frequency = result.at(iter_item->itemName);
                iter_item++;
            }
        }
        iter_transaction->itemList.sort(FPnode_greater_than);  // 排序
        for (
            ItemList::iterator iter_item = iter_transaction->itemList.begin();
            iter_item != iter_transaction->itemList.end();
            iter_item++) {  // 重置频率
            iter_item->frequency = 1;
        }
    }
    for (
        TransactionSet::iterator iter_transaction = transactions.begin();
        iter_transaction != transactions.end();
        iter_transaction++) {  // 重装频繁项集
        vector< FPnode > tmp_fpnode(iter_transaction->itemList.begin(), iter_transaction->itemList.end());
        reRankResult.push_back(tmp_fpnode);
    }
    return reRankResult;
}

HeaderTable make_header_table(
    ItemFreq const &itemFreq,
    int const &threshold) {
    HeaderTable headerTable;
    for (
        ItemFreq::const_iterator iter_itemFreq = itemFreq.cbegin();
        iter_itemFreq != itemFreq.cend();
        iter_itemFreq++) {  // 装填headerTable
        HeaderItem tmp_headerItem;
        tmp_headerItem.itemName = iter_itemFreq->first;
        tmp_headerItem.frequency = iter_itemFreq->second;
        tmp_headerItem.phead.clear();
        headerTable.push_back(tmp_headerItem);
    }
    sort(headerTable.begin(), headerTable.end());  // 降序排列
    return headerTable;
}

int pos_in_header_table(
    string item,
    HeaderTable const &headerTable) {
    int pos = NON_EXIST;
    for (int p = 0; p != headerTable.size(); p++) {
        if (headerTable[p].itemName == item) {
            pos = p;
        }
    }
    return pos;
};

int pos_in_children(
    FPnode node,
    Children const &children) {
    int pos = NON_EXIST;
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->itemName == node.itemName) {
            pos = i;
        }
    }
    return pos;
};

FPnode *create_FP_tree(
    FPset const &freqNodes,
    HeaderTable &headerTable) {
    FPnode *headerNode = new FPnode;
    for (int i = 0; i < freqNodes.size(); i++) {  // 对于一条transaction
        FPnode *nodeInFP = headerNode;            // 从头结点开始构建
        int j = 0;
        while (j != freqNodes[i].size()) {                                        // 对于一条transaction中的item
            int pos = pos_in_children(freqNodes[i][j], nodeInFP->children);       // 寻找是否存在
            if (pos != NON_EXIST) {                                               // 若存在
                nodeInFP->children[pos]->frequency += freqNodes[i][j].frequency;  // 增加频率
                nodeInFP = nodeInFP->children[pos];                               // 接着往下找
                j++;
            } else {                                // 若不存在
                while (j != freqNodes[i].size()) {  // 创建一条新的分支
                    FPnode *newNode = new FPnode;
                    *newNode = freqNodes[i][j];
                    int pos = pos_in_header_table(newNode->itemName, headerTable);
                    headerTable[pos].phead.push_back(newNode);  // 存入phead
                    nodeInFP->children.push_back(newNode);      // 存入分支
                    newNode->parent = nodeInFP;                 // 接着往下找
                    nodeInFP = newNode;
                    j++;
                }
                break;
            }
        }
    }
    return headerNode;
};

vector< pair< string, unsigned int > > mine_FP_node(
    HeaderItem const &headerItem,
    int threshold) {
    ItemFreq itemFreq;  // 记录FP子树中项目的频次
    for (
        ItemPtrList::const_iterator iter_fpnode = headerItem.phead.cbegin();
        iter_fpnode != headerItem.phead.cend();
        iter_fpnode++) {  // 遍历该项链表
        FPnode *fpnode = *iter_fpnode;
        FPnode *leafNode = new FPnode();
        leafNode->parent = fpnode->parent;
        unsigned int nodeFreq = fpnode->frequency;  // 记录叶子结点频次
        while (leafNode->parent != nullptr) {       // 回溯
            leafNode = leafNode->parent;
            if (leafNode->itemName != "") {
                itemFreq[leafNode->itemName] += nodeFreq;  // 累计频次
            }
        }
    }
    delete_unfrequent_item(itemFreq, threshold);  // 删除不频繁项
    vector< pair< string, unsigned int > > itemFreqVec(itemFreq.begin(), itemFreq.end());
    sort(itemFreqVec.begin(), itemFreqVec.end(), ItemFreq_less_than);  // 按照频次降序排列
    return itemFreqVec;
}

void gen_subsets(
    vector< pair< string, unsigned int > > const &input,
    vector< pair< vector< string >, unsigned int > > &result,
    vector< string > &current,
    int index) {
    if (!current.empty()) {  // 如果当前子集非空，则将其加入结果
        result.push_back(make_pair(current, input[index - 1].second));
    }
    for (int i = index; i < input.size(); ++i) {     // 循环在index==input.size()时终止，递归结束
        current.push_back(input[i].first);           // 选择当前元素
        gen_subsets(input, result, current, i + 1);  // 递归生成后续子集
        current.pop_back();                          // 回溯，移除当前元素
    }
}

vector< pair< vector< string >, unsigned int > > get_subsets(
    vector< pair< string, unsigned int > > const &input) {
    vector< pair< vector< string >, unsigned int > > result;
    vector< string > current;
    gen_subsets(input, result, current, 0);
    return result;
}

unordered_map< string, int > OCCU;
void gen_dot_file_DFS(
    FPnode *node,
    ofstream &file) {
    if (node == nullptr) {  // 递归终止
        return;
    }
    string nodeName = node->itemName;
    if (nodeName == "") {  // 如果是根结点则打印root
        nodeName = "root";
    }
    for (
        vector< FPnode * >::const_iterator iter_FPnode = node->children.cbegin();
        iter_FPnode != node->children.cend();
        iter_FPnode++) {
        FPnode *child = *iter_FPnode;
        if (child->children.empty()) {  // 记录叶子结点用以区分
            OCCU[child->itemName]++;
        }
        file << "    \"";
        file << nodeName;
        if (OCCU[node->itemName] != 0) {
            file << "_" << OCCU[node->itemName];
        }
        file << ":" << node->frequency;
        file << "\" -> \"";
        file << child->itemName;
        if (OCCU[child->itemName] != 0) {
            file << "_" << OCCU[child->itemName];
        }
        file << ":" << child->frequency;
        file << "\";" << endl;
        gen_dot_file_DFS(child, file);
    }
}

void gen_dot_file(
    FPnode *root,
    string const &filename) {
    ofstream file(filename);
    file << "digraph G {" << endl;
    gen_dot_file_DFS(root, file);  // 调用深度优先搜索
    file << "}" << endl;
    file.close();
}