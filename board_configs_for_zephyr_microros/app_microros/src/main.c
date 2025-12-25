#include <version.h>

#if ZEPHYR_VERSION_CODE >= ZEPHYR_VERSION(3, 1, 0)
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/posix/sys/time.h>
#else
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <posix/time.h>
#include <zephyr.h>
#endif

#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <std_msgs/msg/int32.h>

#include <rclc/executor.h>
#include <rclc/rclc.h>

#include <microros_transports.h>
#include <rmw_microros/rmw_microros.h>

// Settings
static const int32_t sleep_time_ms = 1000;

#define RCCHECK(fn)                                                            \
  {                                                                            \
    rcl_ret_t temp_rc = fn;                                                    \
    if ((temp_rc != RCL_RET_OK)) {                                             \
      printk("Failed status on line %d: %d. Aborting.\n", __LINE__,            \
             (int)temp_rc);                                                    \
      for (;;) {                                                               \
      };                                                                       \
    }                                                                          \
  }
#define RCSOFTCHECK(fn)                                                        \
  {                                                                            \
    rcl_ret_t temp_rc = fn;                                                    \
    if ((temp_rc != RCL_RET_OK)) {                                             \
      printk("Failed status on line %d: %d. Continuing.\n", __LINE__,          \
             (int)temp_rc);                                                    \
    }                                                                          \
  }

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

void timer_callback(rcl_timer_t *timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    msg.data++;
  }
}

int main(void) {

  k_msleep(10 * sleep_time_ms);

  printk("Reached main()\n");
  printk("Before rmw_uros_init()\n");
  rmw_uros_set_custom_transport(true, NULL, zephyr_transport_open,
                                zephyr_transport_close, zephyr_transport_write,
                                zephyr_transport_read);

  rcl_allocator_t allocator = rcl_get_default_allocator();
  // Initialize and modify options (Set DOMAIN ID to 10)
  rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
  rcl_init_options_init(&init_options, allocator);
  rcl_init_options_set_domain_id(&init_options, 10);
  rclc_support_t support;

  // create init_options
  printk("checking if support init OK\n");
  rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
  printk("support init OK\n");
  // create node
  rcl_node_t node = rcl_get_zero_initialized_node();
  RCCHECK(rclc_node_init_default(&node, "zephyr_publisher", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
      &publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
      "/zephyr_int_publisher"));

  // create timer,
  rcl_timer_t timer;
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(timer_timeout),
                                  timer_callback));

  // create executor
  rclc_executor_t executor;
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  msg.data = 0;

  while (1) {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
    k_sleep(K_MSEC(1));
  }

  // free resources
  RCCHECK(rcl_publisher_fini(&publisher, &node))
  RCCHECK(rcl_node_fini(&node))
  RCCHECK(rcl_init_options_fini(&init_options));
  return 0;
}
