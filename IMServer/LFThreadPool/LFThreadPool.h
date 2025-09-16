#include "../config.h"

#include "../Ckernel.h"


class LFThreadPool {
 public:
  LFThreadPool(size_t thread_count);
  ~LFThreadPool();
  // 提交任务
  //
  void submit(const Task& task);
  //发送网络包
  void submit(const TaskData& task);

 private:
  // 线程主循环：先选举身份，然后进入相应循环
  void threadLoop();

  // 领导者循环：等待新任务，拿到任务后选举继任者，自身降级为“工人”执行任务
  void leaderLoop(std::unique_lock<std::mutex>& lk);

  // 跟随者循环：等待被提升为领导者
  void followerLoop(std::unique_lock<std::mutex>& lk);

  // 管理成员
  std::vector<std::thread> threads;
  std::mutex mtx;
  std::condition_variable task_cv;      // 领导者等待新任务
  std::condition_variable follower_cv;  // 跟随者等待领导者职位
  bool leader_exists;
  bool stopping;
#ifdef NetWork
  std::deque<TaskData> task_queue_data;
#elif
  std::deque<Task> task_queue;
#endif  // NetWork
};
