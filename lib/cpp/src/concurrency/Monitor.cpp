#include "Monitor.h" 
#include "Exception.h" 
#include "Util.h"

#include <assert.h>
#include <errno.h>

#include <iostream>

#include <pthread.h>

namespace facebook { namespace thrift { namespace concurrency { 

/**
 * Monitor implementation using the POSIX pthread library
 * 
 * @author marc
 * @version $Id:$
 */
class Monitor::Impl {

 public:

  Impl() :
    mutexInitialized_(false),
    condInitialized_(false) {
    
    try {
      assert(pthread_mutex_init(&pthread_mutex_, NULL) == 0);
      mutexInitialized_ = true;
      assert(pthread_cond_init(&pthread_cond_, NULL) == 0);
      condInitialized_ = true;
    } catch(...) {
      cleanup();
    }
  }

  ~Impl() { cleanup(); }

  void lock() const { pthread_mutex_lock(&pthread_mutex_); }

  void unlock() const { pthread_mutex_unlock(&pthread_mutex_); }

  void wait(long long timeout) const {

    // XXX Need to assert that caller owns mutex
    assert(timeout >= 0LL);
    if (timeout == 0LL) {
      assert(pthread_cond_wait(&pthread_cond_, &pthread_mutex_) == 0);
    } else {
      struct timespec abstime;
      long long now = Util::currentTime();
      Util::toTimespec(abstime, now + timeout);
      int result = pthread_cond_timedwait(&pthread_cond_,
                                          &pthread_mutex_,
                                          &abstime);
      if (result == ETIMEDOUT) {
	assert(Util::currentTime() >= (now + timeout));
      }
    }
  }

  void notify() {
    // XXX Need to assert that caller owns mutex
    assert(pthread_cond_signal(&pthread_cond_) == 0);
  }

  void notifyAll() {
    // XXX Need to assert that caller owns mutex
    assert(pthread_cond_broadcast(&pthread_cond_) == 0);
  }

 private:

  void cleanup() {
    if (mutexInitialized_) {
      mutexInitialized_ = false;
      assert(pthread_mutex_destroy(&pthread_mutex_) == 0);
    }

    if (condInitialized_) {
      condInitialized_ = false;
      assert(pthread_cond_destroy(&pthread_cond_) == 0);
    }
  }

  mutable pthread_mutex_t pthread_mutex_;
  mutable bool mutexInitialized_;
  mutable pthread_cond_t pthread_cond_;
  mutable bool condInitialized_;
};

Monitor::Monitor() : impl_(new Monitor::Impl()) {}

Monitor::~Monitor() { delete impl_; }

void Monitor::lock() const { impl_->lock(); }

void Monitor::unlock() const { impl_->unlock(); }

void Monitor::wait(long long timeout) const { impl_->wait(timeout); }

void Monitor::notify() const { impl_->notify(); }

void Monitor::notifyAll() const { impl_->notifyAll(); }

}}} // facebook::thrift::concurrency