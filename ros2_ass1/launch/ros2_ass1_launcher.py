import launch
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
def generate_launch_description():
    """Generate launch description with multiple components."""
    container = ComposableNodeContainer(
        name='my_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container',
        composable_node_descriptions=[
            ComposableNode(
                package='ros2_ass1',
                plugin='ros2_ass1::Server_ros2',
                name='robot_fsm_launch'),
            ComposableNode(
                package='ros2_ass1',
                plugin='ros2_ass1::Server_rdm_position',
                name='position_rdm_launch')],
        output='screen',
    )
    return launch.LaunchDescription([container])
