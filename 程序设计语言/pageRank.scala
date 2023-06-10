import org.apache.spark.{HashPartitioner, SparkConf, SparkContext}

// Create RDD
   val links = sc.parallelize(List(
      ("A",List("B","C","D")),
      ("B",List("A")),
      ("C",List("A", "B")),
      ("D",List("B","C"))
    )).partitionBy(new HashPartitioner(10)).persist()

    var ranks = links.mapValues(v => 1.0)

// For to Compute
    for(i <- 0 until 10){
      val contributions = links.join(ranks).flatMap{
        case(pageId, (links, rank)) =>links.map(link => (link, rank / links.size))
      }
      ranks = contributions.reduceByKey((x,y) => x+y).mapValues(v => 0.15 + 0.85*v)
    }

    ranks.collect().foreach(println)
// Save File
    ranks.saveAsTextFlie("ranks")