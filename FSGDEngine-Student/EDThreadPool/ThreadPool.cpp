#include "precompiled.h"
#include "ThreadPool.h"

// these dependencies can be whacked when test objects are taken out for real tasks
#include "WorkerExample.h"
#include "../EDUtilities/InternalOutput.h"

#define WAIT_FOR_THREAD(r) if ((r)->joinable()) (r)->join();

namespace EDThreadPool
{
	ThreadPool* ThreadPool::GetInstance()
	{
		static ThreadPool thread_pool;
		return &thread_pool;
	}

	ThreadPool::~ThreadPool()
	{
	}

	ThreadPool::ThreadPool()
	{
		Init();
	}

	void ThreadPool::Init()
	{
		// TODO-STUDENT - implement your own version
		//InitSolution();
		num_threads = std::thread::hardware_concurrency();
		if (num_threads == 0)
			num_threads = 1;
		run = true;
		for (unsigned int i = 0; i < num_threads; i++)
		{
			std::thread* help = new std::thread(&ThreadPool::Execute, this);
			threads.push_back(help);
		}
		
	}

	void ThreadPool::Shutdown()
	{
		ClearWork();

		// TODO-STUDENT - implement your own version
		//ShutdownSolution();
		queue_mutex.lock();

		run = false;
		work_condition.notify_all();
		queue_mutex.unlock();

		for (unsigned int i = 0; i < num_threads; i++)
			WAIT_FOR_THREAD(threads[i]);

		for (unsigned int i = 0; i < num_threads; i++)
			delete threads[i];

		// if any test objects were created clear them out before we quit
		for(auto &t : testObjects)
		{
			delete t;
		}
		testObjects.clear();
	}

	void ThreadPool::Execute(void)
	{
		// TODO-STUDENT - implement your own version
		//ExecuteSolution();
		unsigned int heapID = EDMemoryManager::MemoryManager::GetInstance()->CreateHeap(MEGABYTE(1));

		while (run.load())
		{
			WorkerThread *work = nullptr;
			GetWork(&work);

			if (work != nullptr)
			{
				work->DoWork();
				work->DoneCallback();
			}
		}

	}

	void ThreadPool::PostWork(WorkerThread* _worker_ptr)
	{
		// TODO-STUDENT - implement your own version
		//PostWorkSolution(_worker_ptr);

		queue_mutex.lock();
		workers.push(_worker_ptr);
		queue_mutex.unlock();
		work_condition.notify_one();
	}

	void ThreadPool::GetWork(WorkerThread** _worker_ptr)
	{
		// TODO-STUDENT - implement your own version
		//GetWorkSolution(_worker_ptr);

		std::unique_lock<std::mutex> locked(queue_mutex);
		work_condition.wait(locked, [&](){return run == false || workers.empty() == false; });

		if (workers.empty())
			*_worker_ptr = nullptr;
		else
		{
			*_worker_ptr = workers.front();
			workers.pop();
		}		
	}

	void ThreadPool::ClearWork()
	{
		// TODO-STUDENT - implement your own version
		//ClearWorkSolution();

		queue_mutex.lock();
		
		while (!workers.empty())
			workers.pop();
		queue_mutex.unlock();
	}

} // end of EDThreadPool namespace
