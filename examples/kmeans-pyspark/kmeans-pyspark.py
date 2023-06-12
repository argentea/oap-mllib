#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""
An example demonstrating k-means clustering.
Run with:
  bin/spark-submit examples/src/main/python/ml/kmeans_example.py

This example requires NumPy (http://www.numpy.org/).
"""
from __future__ import print_function
import sys
sys.path.append('..')
from utils.utils import Timer

# $example on$
from pyspark.ml.clustering import KMeans
from pyspark.ml.evaluation import ClusteringEvaluator
# $example off$


from pyspark.sql import SparkSession

if __name__ == "__main__":
    timer = Timer("KMeans")
    timer.record("Start")

    spark = SparkSession\
        .builder\
        .appName("KMeansExample")\
        .getOrCreate()

    if (len(sys.argv) != 2) :
        print("Require data file path as input parameter")
        sys.exit(1)

    # $example on$
    # Loads data.
    # INIT end
    timer.record("Init")
    # Preprocessing start
    dataset = spark.read.format("libsvm").load(sys.argv[1])

    # Trains a k-means model.
    kmeans = KMeans().setK(2).setSeed(1)

    # Preprocessing end
    model = kmeans.fit(dataset)
    # Convertion + training

    # Make predictions
    predictions = model.transform(dataset)

    # Evaluate clustering by computing Silhouette score
    evaluator = ClusteringEvaluator()

    silhouette = evaluator.evaluate(predictions)
    print("Silhouette with squared euclidean distance = " + str(silhouette))

    # Shows the result.
    centers = model.clusterCenters()
    print("Cluster Centers: ")
    for center in centers:
        print(center)
    # $example off$

    spark.stop()
    # Postprocessing
    timer.record("Postprocessing")
    timer.printTimeTable()

