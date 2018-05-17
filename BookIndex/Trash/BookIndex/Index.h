#pragma once

#define MaxTokens 1024

#include "Book.h"

using namespace std;

struct item {
	int index;
	item* next;
};

struct Token {
	string word;
	item* head;
};

class Index{
public:
	Index(int size) {
		wordlist = new Token[size];
		wordlist_size = size;
		alphabet = new Token[27];			//26����ĸ������
	};
	~Index() {
		delete[] wordlist;
		wordlist = nullptr;
		delete[] alphabet;
		alphabet = nullptr;
	};
	Token* wordlist;
	//���Ա�洢�ؼ���
	Token* alphabet;
	//��26��Ӣ����ĸ�Ŀ�ʼ��������
	int wordlist_size;
	friend Book;
};

//����������˳������ܺܺõ��ṩ���ٷ��ʵĹ��ܣ��ٶ��ϲ����������죨ͬ����������Ϊ����Ҫ����Ƚϲ��ҹؼ���
//ʵ�����޷�����˳��������ȡ�����Ʒ���ֻ�в���ɾ��������ȱ������
//���ܱȽϺõķ���������������
//�⼸���Ȳ�����ϡ�����


