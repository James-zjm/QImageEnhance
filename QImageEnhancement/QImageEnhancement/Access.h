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
	int id; // ʹ��int���ͣ���ΪҪ����ֵΪ256��pseudo-EOF
	std::string code;
	Huffman_node  *left,*right,*parent;
};

typedef Huffman_node* Node_ptr;

class Huffman
{
private:
	Node_ptr node_array[MAX_SIZE]; // Ҷ�ӽڵ�����
	Node_ptr root;  // ���ڵ�
	int size;  // Ҷ�ӽڵ���
	int width, height;//ͼ��//��
	std::fstream fr, fw; // ���롢����ļ���
	std:: map<int, std::string> table;  // �ַ�->huffman����ӳ���

	struct Compare
	{
		bool operator()(const Node_ptr a, const Node_ptr b)
		{
			return a->freq > b->freq;
		}
	};

	// ���ڱȽ����ȶ�����Ԫ�ؼ��˳��
	std::priority_queue <Node_ptr,std::vector<Node_ptr>, Compare > pq;

	// ���������ļ���������ַ�����Ƶ�ʵ�����
	void create_node_array(QImage &image);

	// ���ݹ���õ�Huffman������Huffmanӳ���
	void create_map_table(const Node_ptr node, bool left);

	// �������ȶ���
	void create_pq(QImage &image);

	// ����Huffman��
	void create_huffman_tree();

	// ����Huffman����
	void calculate_huffman_codes();

	// ��ʼѹ������
	void do_compress(QImage &image);

	// ��huffman�����ļ����ؽ�huffman��
	void rebuid_huffman_tree();

	// �����ؽ��õ�huffman�������ļ�
	QImage decode_huffman();

public:
	// ����������������ʼ������
	Huffman(const char * in_file_name, const char * out_file_name);

	// ��������
	~Huffman();

	// ѹ���ļ�
	void compress(QImage &image);

	// ��ѹ�ļ�
	QImage decompress();
};

class RunLength
{
private:
	unsigned int width, height;
	FILE *fr, *fw; // ���롢����ļ���

public:
	RunLength(const char * in_file_name, const char * out_file_name);
	void compress(QImage &image);
	QImage decompress();
};