#ifndef CPP11_BASED_SEMAPHORE_H
#define CPP11_BASED_SEMAPHORE_H
#include <mutex>
#include <condition_variable>
 
namespace LogFileWriterSynchronizationPrimitives
{
	// binary semaphore
	class Semaphore {
	private:
		Semaphore operator=(Semaphore&){}
		Semaphore (Semaphore&){}
		static const int count_max = 1;
		int count_;
		std::mutex m_;
		std::condition_variable cv_;
	public:
		explicit Semaphore(int init_count = count_max);
		// P-operation / acquire
		void wait();
		bool try_wait();
		// V-operation / release
		void signal();
 
		// Lockable requirements
		void lock();
		bool try_lock();
		void unlock();

	};
}
#endif
