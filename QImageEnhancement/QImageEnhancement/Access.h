#pragma once
#include <string>
#include <cstdlib>
#include <string>
#include <string.h>
#include <queue>
#include <map>
#include <vector>
#include <fstream>
#include "image.h"
const int MAX_SIZE = 257;
struct Huffman_node
{
	unsigned int freq;
	int id; // 使用int类型，因为要插入值为256的pseudo-EOF
	std::string code;
	Huffman_node  *left,*right,*parent;
};

typedef Huffman_node* Node_ptr;

class Huffman
{
private:
	Node_ptr node_array[MAX_SIZE]; // 叶子节点数组
	Node_ptr root;  // 根节点
	int size;  // 叶子节点数
	int width, height;//图像//列
	std::fstream fr, fw; // 输入、输出文件流
	std:: map<int, std::string> table;  // 字符->huffman编码映射表

	struct Compare
	{
		bool operator()(const Node_ptr a, const Node_ptr b)
		{
			return a->freq > b->freq;
		}
	};

	// 用于比较优先队列中元素间的顺序
	std::priority_queue <Node_ptr,std::vector<Node_ptr>, Compare > pq;

	// 根据输入文件构造包含字符及其频率的数组
	void create_node_array(QImage &image);

	// 根据构造好的Huffman树建立Huffman映射表
	void create_map_table(const Node_ptr node, bool left);

	// 构造优先队列
	void create_pq(QImage &image);

	// 构造Huffman树
	void create_huffman_tree();

	// 计算Huffman编码
	void calculate_huffman_codes();

	// 开始压缩过程
	void do_compress(QImage &image);

	// 从huffman编码文件中重建huffman树
	void rebuid_huffman_tree();

	// 根据重建好的huffman树解码文件
	QImage decode_huffman();

public:
	// 根据输入和输出流初始化对象
	Huffman(const char * in_file_name, const char * out_file_name);

	// 析构函数
	~Huffman();

	// 压缩文件
	void compress(QImage &image);

	// 解压文件
	QImage decompress();
};

class RunLength
{
private:
	unsigned int width, height;
	FILE *fr, *fw; // 输入、输出文件流

public:
	RunLength(const char * in_file_name, const char * out_file_name);
	void compress(QImage &image);
	QImage decompress();
};