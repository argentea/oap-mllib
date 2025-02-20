#!/usr/bin/env bash

set -x

HADOOP_VERSION=3.2.0
SPARK_VERSION=3.2.0
SPARK_HADOOP_VERSION=hadoop3.2

export HADOOP_HOME=~/opt/hadoop-$HADOOP_VERSION
export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HADOOP_HOME/lib/native

export SPARK_HOME=~/opt/spark-$SPARK_VERSION-bin-$SPARK_HADOOP_VERSION
export PYTHONPATH=$SPARK_HOME/python:$PYTHONPATH
export PYSPARK_PYTHON=python3

export PATH=$HADOOP_HOME/bin:$SPARK_HOME/bin:$PATH
export FI_TCP_IFACE=eth0

set +x
