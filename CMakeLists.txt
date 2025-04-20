cmake_minimum_required(VERSION 3.8)
project(pi_info_stream)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED) # Required for parsing msg folder
find_package(pi_info_stream REQUIRED)

add_executable(pi_info src/pi_info.cpp)

# include_directories(include)
target_include_directories(pi_info PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_compile_features(pi_info PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  pi_info
  "rclcpp"
  "std_msgs"
  "pi_info_stream"
)

#Parse the custom messages
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/SystemStats.msg"
)

# Export dependencies
ament_export_dependencies(rosidl_default_runtime)


install(TARGETS pi_info
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
