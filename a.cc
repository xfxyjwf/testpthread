#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>

using namespace std;

int main() {
  rlimit limit;
  int ret = getrlimit(RLIMIT_STACK, &limit);
  cout << "ret = " << ret << endl;
  cout << "limit: " << limit.rlim_cur << ", " << limit.rlim_max << endl;
  cout << "infinity: " << RLIM_INFINITY << endl;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_getattr_np(pthread_self(), &attr);

  void* addr;
  size_t size;
  ret = pthread_attr_getstack(&attr, &addr, &size);
  cout << "ret = " << ret << endl;
  cout << "addr = " << addr << endl;
  cout << "size = " << size << endl;

  uint8_t* stack_bottom = static_cast<uint8_t*>(addr);
  uint8_t* stack_top = stack_bottom + size;
  size_t kPageSize = 4096;
  size_t sum = 0;
  for (uint8_t* p = stack_top; p >= stack_bottom; p -= kPageSize) {
    cout << "reading " << (void*) p << endl;
    sum += *p;
  }
  cout << "sum = " << sum << endl;

  ret = pthread_attr_getguardsize(&attr, &size);
  cout << "ret = " << ret << endl;
  cout << "guardsize = " << size << endl;

  pthread_attr_destroy(&attr);
}
