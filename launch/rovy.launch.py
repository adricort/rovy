import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_xml.launch_description_sources import XMLLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    rviz_config_file = ( 
        get_package_share_directory(
            "rovy") + "/config/rovy.rviz"
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='log',
        arguments=['-d',rviz_config_file]
    )

    micro_ros_node = Node(
        package='micro_ros_agent',
        executable='micro_ros_agent',
        name='micro_ros_agent',
        output='screen',
        arguments=['serial',
                    '--dev',
                    '/dev/ttyUSB0',
                    '-v6']
    )
    
    realsense_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(
                get_package_share_directory("realsense2_camera"),
                'launch','rs_launch.py'
            )
        ]),
        launch_arguments={
            'enable_pointcloud': 'true',
            'device_type': 'd435'
        }.items()
    )
    
    ds4_driver_launch = IncludeLaunchDescription(
        XMLLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory("ds4_driver"),
                'launch','ds4_driver.launch.xml'
            )
        )
    )

    return LaunchDescription([
        #rviz_node,
        micro_ros_node,
        #realsense_launch,
        ds4_driver_launch
    ])