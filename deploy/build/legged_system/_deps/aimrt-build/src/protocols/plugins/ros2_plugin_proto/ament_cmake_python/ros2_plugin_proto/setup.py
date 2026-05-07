from setuptools import find_packages
from setuptools import setup

setup(
    name='ros2_plugin_proto',
    version='0.1.0',
    packages=find_packages(
        include=('ros2_plugin_proto', 'ros2_plugin_proto.*')),
)
