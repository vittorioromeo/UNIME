


# HBase

## Overview

### History

* Developed for massive natural language data search.

* Open-source implementation of Google BigTable.

    * Semi-structured data. 

    * Cheap, horizontal scalability.

    * Integration with MapReduce.

* Developed as part of Hadoop, on top of HDFS.

### Characteristics

* Non-relational, distributed.

* Column-oriented.

* Multi-dimensional.

* High availability and performance.


## Data model

* **Sparse**, **multi-dimensional**, **sorted** map.

    * {row, column, timestamp} -> cell

* Rows are **lexicographically sorted** on row key.

* **Region**: contiguous set of sorted rows.

### Operators

* Operations are based on **row keys**.

* Single-row operations:

    * Put.

    * Get.

    * Scan.

* Multi-row operations:

    * Scan.

    * MutiPut.

* No joins - use MapReduce.


## Physical structures

* **Region**: unit of distribution and availability.

    * Split when grown too large.

    * Max size is a tuning parameter.

* Row keys are **plain byte arrays**.

* No support for secondary indexes.

    * Create new table with index and exploit sorting for complex queries.
    
    * Use libraries such as **Lily**.

## System architecture

### Components

* The **HMaster** talks to $n$ **HRegionServer** instances.

* HRegionServers contain **HRegion** instances.

* HRegions contain **HLog** and multiple **memstores**.

* The memstores contain **StoreFiles** which are **HFiles** that interact with Hadoop.



## ACID properties

* HBase is **not ACID compliant**.

* Guarantees:

    * Atomicity:

        * All mutations are atomic within a row.

    * Consistency and Isolation:

        * Eventual Consistency.

    * Durability:

        * All visible data is durable data.

## Examples

```sql
create 'impiegato', 'personali', 'professionali'

scan 'impiegato'

drop 'impiegato'

put 'impiegato', 'row1', 'personali:nome', 'mario'

put 'impiegato', 'row1', 'personali:cognome', 'rossi'

put 'impiegato', 'row1', 'personali:eta', '65'

get 'impiegato', 'row1', {COLUMN => ['personali:nome',
    'personali:eta']}
```