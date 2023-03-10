/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.apache.spark.examples.ml

// scalastyle:off println

// $example on$
import org.apache.spark.ml.clustering.KMeans
import org.apache.spark.ml.regression.LinearRegression
import org.apache.spark.ml.evaluation.ClusteringEvaluator
// $example off$
import org.apache.spark.sql.SparkSession

/**
 * An example demonstrating k-means clustering. 
 * Slightly modified from MLlib KMeansExample to add data file path as input parameter
 */
object KMeansExample {

  def main(args: Array[String]): Unit = {
    val spark = SparkSession
      .builder
      .appName(s"${this.getClass.getSimpleName}")
      .getOrCreate()

    if (args.length != 1) {
      println("Require data file path as input parameter")
      sys.exit(1)
    }

    // $example on$
    // Loads data.
    val dataset = spark.read.format("libsvm").load(args(0))
    dataset.cache()

    // Trains a k-means model.
    println("seem good0")
    val kmeans = new KMeans().setK(2).setSeed(1L).setInitMode("random").setMaxIter(5)
    val lr = new LinearRegression().setMaxIter(10).setRegParam(0.3).setElasticNetParam(0.8)
    println("seem good1")
    val lrModel = lr.fit(dataset)
    println("seem good2")
    val model = kmeans.fit(dataset)
    println("seem good3")

    // Make predictions
    //val predictions = model.transform(dataset)

    // Evaluate clustering by computing Silhouette score
    //val evaluator = new ClusteringEvaluator()

    //val silhouette = evaluator.evaluate(predictions)
    println(s"Silhouette with squared euclidean distance = ")

    // Shows the result.
    println("Cluster Centers: ")
    //model.clusterCenters.foreach(println)
    // $example off$

    spark.stop()
  }
}
// scalastyle:on println
