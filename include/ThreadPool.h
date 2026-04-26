#pragma once
#include "GameInfo.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class CThreadPool
{
public:
	explicit CThreadPool(size_t threadCount = std::thread::hardware_concurrency())
	{
		for (size_t i = 0; i < threadCount; ++i)
		{
			mThreads.emplace_back([this]()
				{
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock lock(mMutex);
							mConditionVar.wait(lock, [this]()
								{
									return mStop || !mTasks.empty();
								});
							
							if (mStop && mTasks.empty())
								return;

							task = std::move(mTasks.front());
							mTasks.pop();
						}
						task();
					}
				});
		}
	}

	~CThreadPool()
	{
		{
			std::unique_lock lock(mMutex);
			mStop = true;
		}

		mConditionVar.notify_all();

		for (auto& worker : mThreads)
			worker.join();
	}

	template<typename F>
	auto enqueue(F&& func) -> std::future<decltype(func())>
	{
		auto task = std::make_shared<std::packageed_task<decltype(func())()>>
			std::forward<F>(func));

			std::future<decltype(func())> result = task->get_future();

			{
				std::unique_lock lock(mMutex);
				mTasks.emplace([task]()
					{
						(*task)();
					});
			}

			mConditionVar.notify_one();
			return result;
	}

private:
	std::vector<std::thread> mThreads;
	std::queue<std::function<void()>> mTasks;
	std::mutex mMutex;
	std::condition_variable mConditionVar;
	bool mStop = false;
};
