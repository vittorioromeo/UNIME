



# Cassandra

TODO: merge with 17?

## Background

* **Cassandra** is an open-source DBMS.


### History

* Created to power **Facebook Inbox Search**.

* Open sourced in 2008 as an Apache Incubator project.


### Motivation and function

* Can handle large amounts of data across multiple servers.

* Mimics relational DBMS, using **triggers** and **lightweight** transactions.

* Raw and simple data structures.

* Focus on availability.


## Design

* Emphasis on **performance** over analysis.


### Data organization

* Rows are organized into tables.

* First component of a table's primary key is the **partition key**.

* Rows are clustered by the remaining columns of the key.

* Columns may be indexed separately from primary key.

* Tables can be altered at runtime without blocking queries.


### P2P clustering

* Decentralized design.

    * Every node has same role.

    * No single point of failure.

    * No bottlenecking.


### Fault tolerance

* Automatic replication and replacement of faulty nodes.

* Distribution over multiple data centers.

* **AP**: availability and partitioning-tolerance.

    * Eventual consistency.



## Data model

### Key-value model

* Cassandra is **column-oriented**.

* **Column families**: sets of key-value pairs inside a **keyspace**.

    * Analogies:

        * A column family is like an SQL table.

        * Key-value pairs are like a SQL row.

* A Cassandra **row** is a sequence of key-value pairs.

* Schema is adjusted as new queries are introduced.

    * No joins.


## CQL examples

### Keyspaces

* Creation:

    ```sql
    CREATE KEYSPACE demo
    WITH replication = {‘class’: ’SimpleStrategy’, replication_factor’: 3};
    ```

* Usage:

    ```sql
    USE demo;
    ```


### Tables

```sql
CREATE TABLE users(    
email varchar,              
bio varchar,                
birthday timestamp,         
active boolean,             
PRIMARY KEY (email));
```

```sql
CREATE TABLE tweets(
email varchar,
time_posted timestamp,
tweet varchar,
PRIMARY KEY (email, time_posted));
```

### Queries

* Insertion:

    ```sql
    INSERT INTO users (email, bio, birthday, active)
    VALUES (‘john.doe@bti360.com’, ‘BT360 Teammate’,
    516513600000, true);
    ```

* Selection:

    ```sql
    SELECT * FROM users;
    SELECT email FROM users WHERE active = true;
    ```



## Architecture

* P2P, distributed.

    * All nodes have he same node.

    * Data partitioned among all nodes in a cluster.

* Custom data replication to ensure fault tolerance.

* Transparent elasticy and scalability.

    * No downtimes.

    * Linear performance increase with addition of nodes.

* High availability. 

    * No single point of failure.

    * Multi-geography/zone aware.

        * Supports multiple geographically dispersed datacenters.

* Data redundancy.

* Partitioning.

    * Nodes structured in **ring topology**.

    * Hashed value of key used to assign it to a node.

    * Nodes move around to alleviate loads.

* **Gossip protocols**.

    * Used for node communication. Inspired by real-life gossiping.

    * Periodic, pairwise node-to-node communication.

        * Low cost.

    * Failure detection:

        * Gossiping tracks heartbeats from other nodes.

        * A **suspicion level** variable is used to detect failures.


## Write operations

### Stages

1. Log data in commit log.

2. Write data to memtable.

3. Flush data from memtable.

4. Store data on disk in SSTables.

#### Memtable

* Data structure in memory. 

* Flushed to disk once a certain size is reached.

* Read operations start looking here.


#### SSTable

* Kept on disk.

* Immutable once written.

* Periodically compacted for performance.


### Consistency

* Read consistency:

    * Number of nodes that must agree before read request returns.

    * **One to all**.

* Write consistency:

    * Number of nodes that must be updated before a write is considered successful.

    * **Any to all**.

    * At **an**, a hinted handoff is all that is needed to return.

* **Quorum**:

    * Middle-ground consistency level.

    * Defined as: $(\mathit{replication_factor} / 2) + 1$.

* Example queries:

    *   ```sql
        INSERT INTO table (column1, …) VALUES (value1, …) 
        USING CONSISTENCY ONE
        ```

    *   ```sql
        INSERT INTO table (column1, …) VALUES (value1, …) 
        USING CONSISTENCY QUORUM
        ```


## Delete operations

### Tombstones

* Deleted data is **marked for deletion**.

* Actual deletion will happen on major compaction or configurable timer.


### Compaction

* Runs periodically to merge multiple SSTables.

    * Reclaims space.

    * Creates new index.

    * Merges keys.

    * Combines columns.

    * Discards tombstones.

    * Improves performance.

* Two types:

    1. Major.

    2. Read-only.


### Anti-entropy

* Ensures synchronization of data across nodes.

* Compares data checksums across neighbors.

* Uses **Merkle trees** *(hash trees)*.

    * Leaves are data, intermediate nodes are hashes.



## Read operations

### Read repair

* On read, nodes are queried until a number of nodes matching specified consistency level is reached.

* If consistency level is not met, nodes are updated with most recent value, which is then returned.

* If consistency level is met, value is returned immediately and old nodes are then updated.

### Bloom filters

* **Bloom filters** are used to check if a value is in a set.

* A value is hashed with multiple algorithms. 

    * Bits of created hashes in a **bit vector** are set to `1`.

* Checking for an element:

    * Hash the element again with same functions, check bits.

        * If the element is not there, it is **certain**.

        * Otherwise, there is a small chance of **false positives**.


## Conclusion

### Advantages

* High performance.

* Decentralization.

* Linear scalability.

* Replication.

* No single points of failure.

* MapReduce support.


### Disadvantages

* No referential integrity.

    * No `JOIN`.

* Limited querying options.

* Sorting data is a design decision.

    * No `GROUP BY`.

* No support for atomic operations.

* *"First think about queries, then data model"*.


### Considerations

* Use Cassandra when you have a lot of data spread across multiple servers.

* Write performance is always excellent, read performance depends on write patterns.

    * Schema must be designed for the queries.

