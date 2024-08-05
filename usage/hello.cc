#include <iostream>

#include "stdexec/execution.hpp"
#include "exec/static_thread_pool.hpp"

using namespace stdexec;
using stdexec::sync_wait;

int main() {
  exec::numa_policy numa{exec::no_numa_policy{}};
  exec::static_thread_pool ctx{8};
  scheduler auto sch = ctx.get_scheduler();

  sender auto begin = schedule(sch);
  sender auto hi_again = then(
    begin,
    [] {
      std::cout << "Hello world! Have an int.\n";
      return 13;
    });

  sender auto add_42 = then(hi_again, [](int arg) { return arg + 42; });
  auto [i] = sync_wait(std::move(add_42)).value();
  std::cout << "Result: " << i << std::endl;

  // Sync_wait provides a run_loop scheduler
  std::tuple<run_loop::__scheduler> t = sync_wait(get_scheduler()).value();
  (void) t;

  auto y = let_value(get_scheduler(), [](auto sched) {
    return on(sched, then(just(), [] {
                std::cout << "from run_loop\n";
                return 42;
              }));
  });
  sync_wait(std::move(y));

  sync_wait(when_all(just(42), get_scheduler(), get_stop_token()));
}
