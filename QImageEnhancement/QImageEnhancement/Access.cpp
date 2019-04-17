#include "Access.h"

void Huffman::create_node_array(QImage &image)
{
	int i, count;
	int freq[MAX_SIZE] = { 0 }; // Ƶ��ͳ������
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
	// ����Ƶ��Ϊ1��pseudo-EOF
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

	// �����Ҷ�ӽڵ㣬����һ������Ч���ڵ㣬��������
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

	// ���ڵ���(����pseudo-EOF) + �������� + ������������д���ļ�
	//��һ��д��ͼ����
	fw << width << std::endl;
	fw << height << std::endl;
	// ��2��д��ڵ�����int��
	fw << size << std::endl;

	// ��3~(size+2)��д��huffman������ÿ��д���ַ�+huffman���룬��"43 00100"
	for (table_it = table.begin(); table_it != table.end(); ++table_it)
	{
		fw << table_it->first << " " << table_it->second << std::endl;
	}

	// ��size+3��д��huffman����
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
				//��huffman��01�����Զ�������д�뵽����ļ�
				for (i = 0; i + 7 < length; i += 8)
				{
					// ÿ��λ01ת����һ��unsigned char���
					// ��ʹ��char�����ʹ��char������λ������ʱ�����λ��Ӱ����
					// ����char��unsigned char�໥ת��������λ������
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
	// �Ѷ��������ļ����Ȳ���pseudo-EOF
	table_it = table.find(256);
	if (table_it != table.end())
		code += table_it->second;
	// �ٴ���β��ʣ���huffman����
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
			// ÿ8λд��һ���ļ�
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
	root->parent = NULL; //�����ʱ��parentûʲô���ˣ����Բ��ø�ֵ����Ϊ�˰�ȫ�����Ǹ�ֵΪ��
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

			// ������˿գ����½�һ���ڵ�
			if (tmp == NULL)
			{
				new_node = new Huffman_node();
				new_node->left = NULL;
				new_node->right = NULL;
				new_node->parent = node;

				// ��������һ��0��1,˵������Ҷ�ӽڵ㣬��Ҷ�ӽڵ㸳��ص�ֵ
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
			// �����Ϊ�գ��ҵ��˸�huffman��������һλ������ȴ�Ѿ�������һ���ڵ㣬��˵��
			// ԭ����huffmaninman���������
			//else if (j == length - 1)
			//{
			//	printf("Huffman code is not valid, maybe the compressed file has been broken.\n");
			//	exit(1);
			//}
			// �����Ϊ�գ����ýڵ�ȴ�Ѿ���Ҷ�ӽڵ㣬˵��Ѱ·���������ַ��ı��봦��huffman����Ҳ����
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
	fr.get(in_char);// �������һ���س�
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
					// int to char�ǰ�ȫ�ģ���λ�ᱻ�ض�
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
//����
void Huffman::compress(QImage &image)
{
	create_pq(image);
	create_huffman_tree();
	calculate_huffman_codes();
	do_compress(image);
}
//δ����
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
