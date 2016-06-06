



# NoSQL and NoSQL types


## NoSQL

* Class of non-relational data storage systems.

    * Types:

        * Document store. *Example:* **MongoDB**.

        * Column based. *Example:* **Cassandra**.

        * Graph. *Example:* **Neo4j**.

        * Key-value.

* Usually do not require fixed schema and do not use joins.

    * Can be distributed.

* One or more ACID properties are relaxed.

    * **BASE** transactions:

        * **Basically available**: failures do not affect the entire system.

        * **Soft state**: data copies may be inconsistent.

        * **Eventually consistent**: consistency is obtained over time.

    * Brewer's **CAP** theorem: a distributed system can support only two of the following:

        * **Consistency**.

        * **Availability**.

        * **Partition tolerance**.

* Compared to SQL: higher scalability and flexibility.


## Motivation

* Explosion of social media sites with huge data needs.

* Explosion of storage needs and cloud-based solutions such as AWS.

* Shift to more dynamic data with frequent schema changes.

### Parallel databases and data stores

* Scaling server applications is easy, but not databases. Possible approaches:

    1. `memcache` or similar caching mechanisms. Limited in scalability.

    2. Use existing parallel databases. Expensive and most of them do not support **OLTP** *(online transaction processing)*.

    3. Build parallel stores with databases underneath.

### Sharding

* Consists in the use of multiple cheap databases.

* **Sharding** can be used to partition and scale RDBMSs.

    * Scales well, but it is **not transparent**.



### Parallel key-value data stores

* Distributed and **transparently** partitionable/scalable.

* No support for joins or constraints.


### Scalability

* Necessary due to big data growth.

* **Vertical scalability** *(scale-up)*: increasing performance of a single machine. 

    * Hard to manage.

    * Possible down times.

* **Horizontal scalability** *(scale-out)*: increase the number of machines.

    * Elastically scalable. 

    * Cheaper.

    * Heterogeneity.

* Issue with **NoSQL** and multiple machines: **coordination** between nodes.




## CAP theorem

### Network partitions

* A **network partition** occurs when a failure of a node splits the network.

### C-A-P

* **Consistency**, **availability** and **partition-resilience**.

* Choose two:

    * **CA**: available and consistent, unless there is a partition.

    * **AP**: a replica provides service even in case of a partition, but can be inconsistent.

    * **CP**: always consistent, but a replica may deny service to prevent inconsistency.

### Log-based transactions

* In order to prevent partial transactions from being committed, a **log** is used.

    * After a crash, different actions are taken depending on the data present in the log.

* **Commit protocols** are used to prevent incoherences.




## NoSQL types


* Key-value stores.

* Column NoSQL databases.

* Document-based.

* Graph databases.

* XML databases.

### Key-value stores

* Extremely simple interface.

* Data model: **key-value pairs**.
    
    * No explicit relationships.

    * No queries-by-data.

    * No set operations.

* Operations: 

    * `insert(k, v)`.

    * `fetch(k)`.

    * `update(k, v)`.

    * `delete(k)`.

* Implementation:

    * Records distributed to nodes depending on key.

    * Replciation.

    * Single-record transactions *(eventual consistency)*.

        * No multi-operation transactions.

* Examples: SimpleDB, Riak.

* Use for: storing session information, user profiles, shopping carts.



### Document stores

* Similar to key-value stores, except that values are **documents**.

* Data model: **key-document pairs**.

    * Document: **JSON**, **XML**, etc...

* Operations: like key-value stores.

* Examples: CouchDB, MongoDB, SimpleDB.

* Use for: event logging, CMSs, analytics, e-commerce.

* *Example:* MongoDB.




### Column-oriented

* Data is stored in **column order**.

    * Key-value pairs can be stored and retrieved in massively parallel systems.

* Data model: **families of attributes** defined in a schema.

* Storing principle: **big hashed distributed tables**.

* Properties:

    * Horizontal and vertical partitioning.

    * High availability.

    * Transparency to application.

* *Example:* Cassandra.


### Graph database

* Data model: **nodes** and **edges**.

* Interface and query languages vary.

* *Examples:* Neo4j, FlockDB, Prgel.