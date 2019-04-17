#include "Access.h"

void Huffman::create_node_array(QImage &image)
{
	int i, count;
	int freq[MAX_SIZE] = { 0 }; // 频数统计数组
	char in_char;
	QColor Color;
	for (int x = 0; x < image.width(); x++)
	{
		for (int y = 0; y < image.height(); y++)
		{
			Color = QColor(image.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			freq[gray]++;
		}
	}

	count = 0;
	for (i = 0; i < MAX_SIZE; ++i)
	{
		if (freq[i] <= 0)
			continue;
		Node_ptr node = new Huffman_node();
		node->id = i;
		node->freq = freq[i];
		node->code = "";
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;

		node_array[count++] = node;
	}
	// 插入频率为1的pseudo-EOF
	Node_ptr node = new Huffman_node();
	node->id = 256;
	node->freq = 1;
	node->code = "";
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;

	node_array[count++] = node;

	size = count;
	width = image.width();
	height = image.height();
}

void Huffman::create_map_table(const Node_ptr node, bool left)
{
	if (left)
		node->code = node->parent->code + "0";
	else
		node->code = node->parent->code + "1";

	// 如果是叶子节点，则是一个“有效”节点，加入编码表
	if (node->left == NULL && node->right == NULL)
		table[node->id] = node->code;
	else
	{
		if (node->left != NULL)
			create_map_table(node->left, true);
		if (node->right != NULL)
			create_map_table(node->right, false);
	}
}

void Huffman::create_pq(QImage &image)
{
	create_node_array(image);
	for (int i = 0; i < size; ++i)
		pq.push(node_array[i]);
}

void Huffman::create_huffman_tree()
{
	root = NULL;

	while (!pq.empty())
	{
		Node_ptr first = pq.top();
		pq.pop();
		if (pq.empty())
		{
			root = first;
			break;
		}
		Node_ptr second = pq.top();
		pq.pop();
		Node_ptr new_node = new Huffman_node();
		new_node->freq = first->freq + second->freq;

		if (first->freq <= second->freq)
		{
			new_node->left = first;
			new_node->right = second;
		}
		else
		{
			new_node->left = second;
			new_node->right = first;
		}
		first->parent = new_node;
		second->parent = new_node;

		pq.push(new_node);
	}
}

void Huffman::calculate_huffman_codes()
{
	if (root == NULL)
	{
		printf("Build the huffman tree failed or no characters are counted\n");
		exit(1);
	}

	if (root->left != NULL)
		create_map_table(root->left, true);
	if (root->right != NULL)
		create_map_table(root->right, false);
}

void Huffman::do_compress(QImage &image)
{
	int length, i, j, byte_count;
	char in_char;
	unsigned char out_c, tmp_c;
	std::string code, out_string;
	std::map<int, std::string>::iterator table_it;

	// 按节点数(包括pseudo-EOF) + 哈夫曼树 + 哈夫曼编码来写入文件
	//第一行写入图像宽高
	fw << width << std::endl;
	fw << height << std::endl;
	// 第2行写入节点数（int）
	fw << size << std::endl;

	// 第3~(size+2)行写入huffman树，即每行写入字符+huffman编码，如"43 00100"
	for (table_it = table.begin(); table_it != table.end(); ++table_it)
	{
		fw << table_it->first << " " << table_it->second << std::endl;
	}

	// 第size+3行写入huffman编码
	code.clear();
	QColor Color;
	int x, y;
	for (x = 0; x < image.width(); x++)
	{
		for (y = 0; y < image.height(); y++)
		{
			Color = QColor(image.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			table_it = table.find(gray);
			if (table_it != table.end())
				code += table_it->second;
			length = code.length();
			if (length > 10)
			{
				out_string.clear();
				//将huffman的01编码以二进制流写入到输出文件
				for (i = 0; i + 7 < length; i += 8)
				{
					// 每八位01转化成一个unsigned char输出
					// 不使用char，如果使用char，在移位操作的时候符号位会影响结果
					// 另外char和unsigned char相互转化二进制位并不变
					out_c = 0;
					for (j = 0; j < 8; j++)
					{
						if ('0' == code[i + j])
							tmp_c = 0;
						else
							tmp_c = 1;
						out_c += tmp_c << (7 - j);
					}
					out_string += out_c;
				}
				fw << out_string;
				code = code.substr(i, length - i);
			}
		}
	}
	// 已读完所有文件，先插入pseudo-EOF
	table_it = table.find(256);
	if (table_it != table.end())
		code += table_it->second;
	// 再处理尾部剩余的huffman编码
	length = code.length();
	out_c = 0;
	for (i = 0; i < length; i++)
	{
		if ('0' == code[i])
			tmp_c = 0;
		else
			tmp_c = 1;
		out_c += tmp_c << (7 - (i % 8));
		if (0 == (i + 1) % 8 || i == length - 1)
		{
			// 每8位写入一次文件
			fw << out_c;
			out_c = 0;
		}
	}
}

void Huffman::rebuid_huffman_tree()
{
	int i, j, id, length;
	std::string code;
	Node_ptr node, tmp, new_node;
	root = new Huffman_node();
	root->left = NULL;
	root->right = NULL;
	root->parent = NULL; //解码的时候parent没什么用了，可以不用赋值，但为了安全，还是赋值为空
	fr >> width >> height;
	fr >> size;
	if (size > MAX_SIZE)
	{
		printf("The number of nodes is not valid, maybe the compressed file has been broken.\n");
		exit(1);
	}

	for (i = 0; i < size; ++i)
	{
		fr >> id;
		fr >> code;

		length = code.length();
		node = root;
		for (j = 0; j < length; ++j)
		{
			if ('0' == code[j])
				tmp = node->left;
			else if ('1' == code[j])
				tmp = node->right;
			//else
			//{
			//	printf("Decode error, huffman code is not made up with 0 or 1\n");
			//	exit(1);
			//}

			// 如果到了空，则新建一个节点
			if (tmp == NULL)
			{
				new_node = new Huffman_node();
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->parent = node;

				// 如果是最后一个0或1,说明到了叶子节点，给叶子节点赋相关的值
				if (j == length - 1)
				{
					new_node->id = id;
					new_node->code = code;
				}

				if ('0' == code[j])
					node->left = new_node;
				else
					node->right = new_node;

				tmp = new_node;
			}
			// 如果不为空，且到了该huffman编码的最后一位，这里却已经存在了一个节点，就说明
			// 原来的huffmaninman是有问题的
			//else if (j == length - 1)
			//{
			//	printf("Huffman code is not valid, maybe the compressed file has been broken.\n");
			//	exit(1);
			//}
			// 如果不为空，但该节点却已经是叶子节点，说明寻路到了其他字符的编码处，huffman编码也不对
			//else if (tmp->left == NULL && tmp->right == NULL)
			//{
			//	printf("Huffman code is not valid, maybe the compressed file has been broken.\n");
			//	exit(1);
			//}
			node = tmp;
		}

	}
}

QImage Huffman::decode_huffman()
{
	bool pseudo_eof;
	int i, id;
	char in_char;
	unsigned char u_char, flag;
	Node_ptr node;

	node = root;
	pseudo_eof = false;
	QImage newImage(width, height, QImage::Format_ARGB32);
	fr.get(in_char);// 跳过最后一个回车
	int x = 0, y = 0;
	while (/*!fr.eof()*/1)
	{
		fr.get(in_char);
		u_char = (unsigned char)in_char;
		flag = 0x80;
		for (i = 0; i < 8; ++i)
		{

			if (u_char & flag)
				node = node->right;
			else
				node = node->left;

			if (node->left == NULL && node->right == NULL)
			{
				id = node->id;
				if (id == 256)
				{
					pseudo_eof = true;
					break;
				}
				else
				{
					// int to char是安全的，高位会被截断
					newImage.setPixel(x, y, qRgb(id, id, id));
					y++;
					if (y == height)
					{
						x++;
						y = 0;
						if (x == width)
							pseudo_eof = true;
					}
					if (x == 128 && y == 185)
					{
						int xxx = 1 + 1;
					}
					//x++;
					//if (x == height)
					//{
					//	y++;
					//	x = 0;
					//}
					node = root;
				}
			}
			flag = flag >> 1;
		}
		if (pseudo_eof)
			break;
	}
	return newImage.copy();
}

Huffman::Huffman(const char * in_file_name, const char * out_file_name)
{
	if (strlen(in_file_name) != 0)
	{
		fr.open(in_file_name, std::ios::in| std::ios::binary);
		if (!fr)
		{
			printf("Open file error, path is: %s\n", in_file_name);
			exit(1);
		}
	}
	if (strlen(out_file_name) != 0)
	{
		fw.open(out_file_name, std::ios::out | std::ios::binary);
		if (!fw)
		{
			printf("Open file error, path is: %s\n", out_file_name);
			exit(1);
		}
	}
	width = 0;
	height = 0;
}

Huffman::~Huffman()
{
	fr.close();
	fw.close();
}
//改完
void Huffman::compress(QImage &image)
{
	create_pq(image);
	create_huffman_tree();
	calculate_huffman_codes();
	do_compress(image);
}
//未改完
QImage Huffman::decompress()
{
	rebuid_huffman_tree();
	return decode_huffman();
}

RunLength::RunLength(const char * in_file_name, const char * out_file_name)
{
	if (strlen(in_file_name) != 0)
	{
		fr = fopen(in_file_name, "rb");
		if (!fr)
		{
			printf("Open file error, path is: %s\n", in_file_name);
			exit(1);
		}
	}
	if (strlen(out_file_name) != 0)
	{
		fw = fopen(out_file_name, "wb");
		if (!fw)
		{
			printf("Open file error, path is: %s\n", out_file_name);
			exit(1);
		}
	}
	width = 0;
	height = 0;
}

void RunLength::compress(QImage & image)
{
	width = image.width();
	height = image.height();
	fwrite(&width, sizeof(int), 1, fw);
	fflush(fw);
	fseek(fw, sizeof(int), SEEK_SET);
	fwrite(&height, sizeof(int), 1, fw);
	fflush(fw);
	fseek(fw, sizeof(int), SEEK_CUR);
	unsigned char code = 0;
	unsigned short len = 0;
	QColor Color;
	Color = QColor(image.pixel(0, 0));
	unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
	code = gray;
	for (int x = 0; x < image.width(); x++)
	{
		for (int y = 0; y < image.height(); y++)
		{
			Color = QColor(image.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			if (code != gray)
			{
				fwrite(&code, sizeof(char), 1, fw);
				fflush(fw);
				fseek(fw, sizeof(char), SEEK_CUR);
				fwrite(&len, sizeof(short), 1, fw);
				fflush(fw);
				fseek(fw, sizeof(short), SEEK_CUR);
				len = 1;
				code = gray;
			}
			else
			{
				len++;
			}
		}
	}

}

QImage RunLength::decompress()
{
	fread(&width, sizeof(int), 1, fr);
	fseek(fr, sizeof(int), SEEK_SET);
	fread(&height, sizeof(int), 1, fr);
	fseek(fr, sizeof(int), SEEK_CUR);
	unsigned char code = 0;
	unsigned short len = 0;
	QColor Color;
	QImage newImage(width, height, QImage::Format_ARGB32);
	int x = 0, y = 0,image_len= 0;
	bool is_eof = false;
	while (image_len < width*height)
	{
		fread(&code, sizeof(char), 1, fr);
		fseek(fr, sizeof(char), SEEK_CUR);
		fread(&len, sizeof(short), 1, fr);
		fseek(fr, sizeof(short), SEEK_CUR);
		for (int i = 0; i < len; i++)
		{
			newImage.setPixel(x, y, qRgb(code, code, code));
			y++;
			image_len++;
			if (y == height)
			{
				x++;
				y = 0;
				if (x == width)
					is_eof = true;
			}
		}
		if (is_eof == true)
			break;
	}
	return newImage;
}
