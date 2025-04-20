#include <cstdio>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "pi_info_stream/msg/system_stats.hpp"
#include "pi_info_stream/pi_info.hpp"
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class InfoStream : public rclcpp::Node // inherit Node class from rclcpp namespace
{
public:
	InfoStream() : Node("Info")
	{
		sysInfoPub = this->create_publisher<pi_info_stream::msg::SystemStats>("UsageStats", 2);
		timer_ = this->create_wall_timer(1000ms, std::bind(&InfoStream::publish_message, this));
	}

private:
	void publish_message()
	{
		auto StatInfo = pi_info_stream::msg::SystemStats();
		StatInfo.sys_info = "Raspberry Pi 3B \nRAM: 1GB \nSwap: 2GB";
		StatInfo.cpu_temp = get_cpu_temp();
		StatInfo.cpu_usage = get_cpu_usage();
		StatInfo.ram_usage = get_memory_usage_percent();
		StatInfo.swap_usage = get_swap_usage_percent();
		sysInfoPub->publish(StatInfo);
	}

	rclcpp::Publisher<pi_info_stream::msg::SystemStats>::SharedPtr sysInfoPub;
	rclcpp::TimerBase::SharedPtr timer_;

};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InfoStream>());
  rclcpp::shutdown();
  return 0;
}
