#include "ppc.h"

void ProduceItemA(ItemRepositoryA *ir, Package item)
{
	std::unique_lock<std::mutex> lock(ir->mtx);
	if(((ir->write_position + 1) % kItemRepositorySize) == ir->read_position)
	{
		// item buffer is full, just return directly
		lock.unlock();
		return;
	}
	(ir->item_buffer)[ir->write_position] = item;
	(ir->write_position)++;
	if(ir->write_position == kItemRepositorySize)
		ir->write_position = 0;
	(ir->repo_not_empty).notify_all(); // 通知消费者仓库不为空
	lock.unlock();
}

void ProduceItemB(ItemRepositoryB *ir, Info item)
{
	std::unique_lock<std::mutex> lock(ir->mtx);
	if(((ir->write_position + 1) % kItemRepositorySize) == ir->read_position)
	{
		// item buffer is full, just return directly
		lock.unlock();
		return;
	}
	(ir->item_buffer)[ir->write_position] = item;
	(ir->write_position)++;
	if(ir->write_position == kItemRepositorySize)
		ir->write_position = 0;
	(ir->repo_not_empty).notify_all(); // 通知消费者仓库不为空
	lock.unlock();
}

Package ConsumeItemA(ItemRepositoryA *ir)
{
	Package data;
	std::unique_lock<std::mutex> lock(ir->mtx);
	while(ir->write_position == ir->read_position) 
	{
		// item buffer is empty, just wait here
		(ir->repo_not_empty).wait(lock); // 消费者等待“缓冲区不为空”这一条件发生
	}
	data = (ir->item_buffer)[ir->read_position];
	(ir->read_position)++;
	if(ir->read_position >= kItemRepositorySize)
		ir->read_position = 0;
	(ir->repo_not_full).notify_all(); // 通知生产者仓库不为满
	lock.unlock();
	return data;
}

Info ConsumeItemB(ItemRepositoryB *ir)
{
	Info data;
	std::unique_lock<std::mutex> lock(ir->mtx);
	while(ir->write_position == ir->read_position) 
	{
		// item buffer is empty, just wait here
		(ir->repo_not_empty).wait(lock); // 消费者等待“缓冲区不为空”这一条件发生
	}
	data = (ir->item_buffer)[ir->read_position];
	(ir->read_position)++;
	if(ir->read_position >= kItemRepositorySize)
		ir->read_position = 0;
	(ir->repo_not_full).notify_all(); // 通知生产者仓库不为满
	lock.unlock();
	return data;
}

void InitItemRepository(ItemRepositoryA *ira, ItemRepositoryB *irb)
{
	ira->write_position = 0;
	ira->read_position = 0;
	irb->write_position = 0;
	irb->read_position = 0;
}
