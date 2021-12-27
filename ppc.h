#ifndef __PPC_H__
#define __PPC_H__

#include <unistd.h>
#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

static const int kItemRepositorySize = 5;

struct Package
{
	short int signal[256 * 8];
	short int power;
};

struct Info
{
	short int power;
	short int theta;
	short int phi;
	std::string onechannel;
	std::string fftchannel;
};

// 定义缓冲区结构体
struct ItemRepositoryA
{
	Package item_buffer[kItemRepositorySize];
	size_t read_position;
	size_t write_position;
	std::mutex mtx; // 互斥量，保护缓冲区
	std::condition_variable repo_not_full; // 条件变量，表示缓冲区不为满
	std::condition_variable repo_not_empty; // 条件变量，指示缓冲区不为空
} gItemRepositoryA;

struct ItemRepositoryB
{
	Info item_buffer[kItemRepositorySize];
	size_t read_position;
	size_t write_position;
	std::mutex mtx; // 互斥量，保护缓冲区
	std::condition_variable repo_not_full; // 条件变量，表示缓冲区不为满
	std::condition_variable repo_not_empty; // 条件变量，指示缓冲区不为空
} gItemRepositoryB;

typedef struct ItemRepositoryA ItemRepositoryA;

typedef struct ItemRepositoryB ItemRepositoryB;

void ProduceItemA(ItemRepositoryA *ir, Package item);

void ProduceItemB(ItemRepositoryB *ir, Info item);

Package ConsumeItemA(ItemRepositoryA *ir);

Info ConsumeItemB(ItemRepositoryB *ir);

void InitItemRepository(ItemRepositoryA *ira, ItemRepositoryB *irb);

#endif
