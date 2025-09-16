#include "LFThreadPool.h"

LFThreadPool::LFThreadPool(size_t thread_count)
    : stopping(false), leader_exists(false) {
  for (size_t i = 0; i < thread_count; ++i) {
    threads.emplace_back(&LFThreadPool::threadLoop, this);
  }
}

LFThreadPool::~LFThreadPool() {
  {
    std::lock_guard<std::mutex> lk(mtx);
    stopping = true;
    // 唤醒所有，退出阻塞
    task_cv.notify_all();
    follower_cv.notify_all();
  }
  for (auto& t : threads) {
    if (t.joinable()) t.join();
  }
}

void LFThreadPool::threadLoop() {
  std::unique_lock<std::mutex> lk(mtx);
  if (!leader_exists) {
    leader_exists = true;
    leaderLoop(lk);
  } else {
    followerLoop(lk);
  }
}

void LFThreadPool::followerLoop(std::unique_lock<std::mutex>& lk) {
  while (!stopping) {
    // 1.等待领导者职位空缺
    follower_cv.wait(lk, [&] { return stopping || !leader_exists; });   //返回true，继续执行；否则进入等待状态，等待被唤醒；
    if (stopping) break;

    // 2.自己被提升为新领导者
    leader_exists = true;
    leaderLoop(lk);
    return;
  }
}


#ifdef NetWork
//发送网络包
void LFThreadPool::submit(const TaskData& task) {
  {
    std::lock_guard<std::mutex> lk(mtx);
    if (stopping) throw std::runtime_error("LFThreadPool is stopping");
    task_queue_data.push_back(task);
  }
  // 唤醒领导者（如果当前确实有领导者在等候）
  std::lock_guard<std::mutex> lk(mtx);
  if (leader_exists) {
    task_cv.notify_one();
  } else {
    // 万一没有领导者，则唤醒一个跟随者来担任新领导者
    follower_cv.notify_one();
  }
}

void LFThreadPool::leaderLoop(std::unique_lock<std::mutex>& lk) {
  while (!stopping) {
    //1.领导者等待 等待新任务
    task_cv.wait(lk, [&] { return stopping || !task_queue_data.empty(); });//返回true，继续执行；否则进入等待状态，等待被唤醒；
    if (stopping) break;

    //2.拿到一个任务
    TaskData task = task_queue_data.front();
    task_queue_data.pop_front();

    //3.选举新的领导者
    leader_exists = false;
    follower_cv.notify_one();//用于通知一个等待中的线程（Follower）来成为新的Leader；（只会唤醒一个不会惊群）
    //相当于消费者生产者模型中，生产者中将任务放在任务队列中通知消费者队列不为空
    //4. 自身变为工人，释放锁去执行任务
    lk.unlock();
    // !todo kernal work
    Ckernel::instance().dealData(task.data,task.len,task.from);
    // task.func(task.data, task.len);
    //std::cout  << '\n'  << '\n' << '\n' << task.data << ' ' << task.len << '\n';
    //std::this_thread::sleep_for(std::chrono::seconds(5));


    // 5.执行完毕，自己重新成为跟随者
    lk.lock();
    followerLoop(lk);
    return;
  }
}

#elif
void LFThreadPool::submit(const Task& task) {
  {
    std::lock_guard<std::mutex> lk(mtx);
    if (stopping) throw std::runtime_error("LFThreadPool is stopping");
    task_queue.push_back(task);
  }
  // 唤醒领导者（如果当前确实有领导者在等候）
  std::lock_guard<std::mutex> lk(mtx);
  if (leader_exists) {
    task_cv.notify_one();
  } else {
    // 万一没有领导者，则唤醒一个跟随者来担任新领导者
    follower_cv.notify_one();
  }
}

void LFThreadPool::leaderLoop(std::unique_lock<std::mutex>& lk) {
  while (!stopping) {
    // 等待新任务
    task_cv.wait(lk, [&] { return stopping || !task_queue.empty(); });
    if (stopping) break;

    // 拿到一个任务
    Task task = task_queue.front();
    task_queue.pop_front();

    // 选举新的领导者
    leader_exists = false;
    follower_cv.notify_one();

    // 自身变为工人，释放锁去执行任务
    lk.unlock();
    task.func(task.data, task.len);
    lk.lock();

    // 执行完毕，自己重新成为跟随者
    followerLoop(lk);
    return;
  }
}

#endif  // NetWork

