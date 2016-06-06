



# Parallel DBMSs and cloud architectures

## Parallelism

* Ideally speeds up computation by a factor of $1 / n$.

* Two types:

    1. **Inter-query**: different queries ran in parallel.

    2. **Intra-query**: parts of the same query *(subqueries)* ran in parallel.

### Relationship with data fragmentation

* Data fragments are in different locations, which can be associated to different processors.


### Speed-up and scale-up

* **Speed-up**: only related to inter-query parallelism. Measures $\mathit{tps}$ as the number of processors grows.

* **Scale-up**: related to both parallelism types. Measures $\frac{\mathit{cost}}{\mathit{tps}}$ aas the number of processors grows.



## Cloud computing architectures

* **Cloud computing** describes a class of network-based computing:

    * A collection/group of networked hardware, software and infrastructure *(platform)*.

    * Uses the Internet for communication/transport, providing hardware and software services to client.

* The complexity of the platforms is hidden behind simple **APIs**.

### Classification

#### Characteristics

* **Remotely hosted**.

* **Ubiquitous**: services/data available from anywhere.

* **Commodified**: pay for what you want/need.

* Massive scale.

* Resilient computing.

* Homogeneity.

* Geographic distribution.

* Virtualization.

* Service-orientation.

* Low-cost.

* Security.

#### Features

* **On-demand self-service**: architecture elements can be defined depending on current needs through web interfaces.

* **Remote access**.

* **Measured services**: architectural resources are rented using costs depending on use.

* **Elasticity**.

* **Resource sharing**.


#### Types

* **Private cloud**: of an organization/institution.

* **Community cloud**: of a community of organizations/institutions.

* **Public cloud**: like AWS or Azure.

* **Hybrid cloud**: private cloud that use public services when needed.

* **Cloud federations**.


### Service models

#### Layers

* From application-focused to infrastructure-focused:

    1. Services.

    2. Application.

    3. Development.

    4. Platform.

    5. Storage.

    6. Hosting.

#### IaaS

* **IaaS**: clients rent only hardware resources.

##### Virtualization

* The basis of IaaS.

* **Virtual workspaces**: abstraction over the execution environment.

    * Has specific resource quota and software configuration.

* Implemented on **VMs** *(virtual machines)*.

    * Abstraction of the physical host.

    * Advantages:

        * OS flexibility. Easier deployment.

        * Versioning/backups/migrations.

* A **VMM** *(virtual machine monitor, or hypervisor)* is used to manage multiple VMs on a single machine.


#### PaaS

* **PaaS**: clients rent hardware resources and base software.

* Deploys user-created applications.

* Highly-scalable architecture.

#### SaaS

* **SaaS**: clients rent finished applications.

* Provides applications.

* *Examples:* Facebook apps, Google apps.


##### Maturity model

* **Level 1**: ad-hoc/custom. One instance per customer.

* **Level 2**: configurable per customer.

* **Level 3**: configurable and multi-tenant-efficient.

* **Level 4**: scalable *(uses load balancer)* level 3.


### Google ecosystem

#### GFS

* Distributed file system. 

* Two node types: 

    * **Chunk**: nodes that store files. 

        * Every file is 64MB. 

        * Every chunk is assigned to a 64bit partition. 

        * Chunks are periodically replicated. 

    * **Master**: manage chunk metadata, 64bit partition tables, chunk copies locations.

#### MapReduce

* Like Hadoop MapReduce.

#### BigTable

* A **key-value** big data system based on GFS.

#### Chubby

* Manages locks and agreements between nodes.

* A **cell** is a small set of servers *(usually 5)* called replicas. 

    * Replicas use the Paxos protocol to elect a master. 

* Similar to Apache Zookeeper.



### Hadoop ecosystem and MapReduce

* **Apache Hadoop** is a suite of open-source components which serve as the building blocks of large distributed systems.

    * Focus on gradual, horizontal scaling.

#### ZooKeeper

* **ZooKeeper** is a distributed coordination service which is used when nodes in a distributed system need a single source of truth.

    * Similar to **Google Chubby**.

* Implemented as a single moveable master, with $n$ coordinated nodes. 
    
    * A majority $(n/2+1)$ must agree on a write.

    * Reads can be answered by any node.

#### HDFS

* **HDFS**: distributed filesystem developed in Java.

    * Uses TCP/IP for communication.

    * Files are fragmented in separate nodes and are replicated.

    * The main node is called **NameNode**, others are called **workers**.

#### MapReduce

* **MapReduce**: parallel computation model.
    
    * **Jobs** are handled by a **job tracker**.

    * Jobs assign **tasks**, which are handled by a **task tracker**.


#### Apache Pig and Pig Latin

* Query system based on Hadoop.

* Data model is similar to OODBMSs, but does not support inheritance.

    * Data is organized in relationships.

    * Relations can contain duplicated elements *(tuple bags)*.

    * There is no explicit primary key.

* Example query: `FOREACH table GENERATE attribute0 attribute1;`.


#### Apache Hive and Hive QL

* Similar to Pig, but closer to SQL.