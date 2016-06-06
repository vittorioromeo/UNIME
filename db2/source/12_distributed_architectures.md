



# Distributed architectures

## Distributed DBMSs

### Basics and data fragmentation

* Based on **autonomy** and **cooperation**.

* Data **fragmentation** and **allocation**:

    * A relation $R$ is split in $R_i$ fragments.

    * **Horizontal** fragmentation:

        * $R_i$: set of tuples with same schema as $R$.

        * Like the `where` SQL clause.

    * **Vertical** fragmentation:

        * $R_i$: set of tuples with subschema of $R$.

        * Like the `select` SQL clause.


### Transparency levels

* **Fragmentation** transparency: independence of a query from data fragmentation and allocation.

* **Allocation** transparency: fragment structure must be specified in a query, but not location.

* **Language** transparency: both fragment structure and location have to be specified in a query.


### Transaction classification

* **Remote request**: readonly *(`select`)* transactions towards a **single** DBMS.

* **Remote transaction**: general transactions towards a **single** DBMS.

* **Distributed transaction**: towards multiple DBMSs, but every SQL operation targets a single DBMS.

* **Distributed request**: arbitrary transaction, language-level transparency.



## Distributed DBMSs technology

### Consistency and persistency

* **Consistency**: does not depend on data distribution. Constraints are only properties local to a specific DBMS. This is a limitation of DBMSs.

* **Persistency**: does not depend on data distribution. Every sistem guarantees persistency thanks to dumps and backups.


### Optimization

* **Global optimization** is performed through a cost analysis.

    * A tree of possible alternatives is examined.

    * **IO**, **CPU** and **bandwidth** coss are taken into account.


### Concurrency control

* *Problem:* two transactions $t_1$ and $t_2$ can be composed of subtransactions whose execution is in conflict.

    * The transactions are **locally serializable**.

    * The transactions are **not globally serializable**.

* **Global serializability**: two transactions are globally serializable if $\exists S$ *(serial schedule)* that is equivalent to every local schedule $S_i$. 

    * For every node $i$, the projection $S[i]$ of $S$ needs to be equivalent to $S_i$

    * This property can fulfilled using **2-phase locking** or **timestamping**.

#### Lamport's method for timestamping

* Every transaction needs a timestamp of the time instant where it needs to be synchronized with other transactions.

* A timestamp is composed by two numbers: **node ID** and **event ID**.

* Nodes have a local counter that helps ordering transactions.


#### Distributed deadlock detection

* Two subtransactions may be waiting for one another in the same or in diffrerent DBMSs.

* A **waiting sequence** can be built for every transaction.

* Algorithm:

    1. DBMSs share their waiting sequences. 

    2. Waiting sequences are composed in a **local waiting graph**.

    3. Deadlocks are detected locally and solved by aborting transactions.

    4. Updated waiting sequences are sent to other DBMSs.



## Distributed transaction atomicity

### 2-phase commit protocol

* Conceptually similar to marriage.

* Servers are called **RMs** *(resource managers)*.

* A coordinator is called **TM** *(transaction manager)*.

* Both RMs and the TM have **local logs**.

* TM log records:

    * `prepare`: contains RMs identities.

    * `global commit/abort`: atomic and persistent decision regarding **the entire transaction**.

    * `complete`: conclusion of the protocol.

* RM log records:

    * `ready`: signals availability of the node.

* Algorithm *(ideal situation)*:

    * Phase one *(preparation)*:

        1. TM sends `prepare`, sets a **timeout** for RM responses.

        2. RMs wait for `prepare` messages. On arrival, they send `ready`. If an RM is in a bad state, `not-ready` is sent instead, terminating the protocol *(`global abort`)*.

        3. TM collects RM messages. On success, sends `global commit`.

    * Phase two:

        1. TM sends global decision, setting a **timeout**.

        2. Ready RMs wait for the decision. On arrival, they either log `commit` or `abort`, and send an `ack` to the TM.

        3. TM collects all `ack` messages. If all of them arrived, `complete` is set. If an `ack` is missing, a new **timeout** is set and transmissions are repeated.

* The period between `ready` and `commit/abort` is called **uncertainty interval** - the protocol tries to minimize its length.


#### Recovery protocols

* RM drops:

    * If last record was `abort`, actions will be undone.

    * If last record was `commit`, actions will be repeated.

    * If last record was `ready`, we are in a **doubtful situation**.

        * Information needs to be requested from TM.

* TM drops:

    * If last record as `prepare`, some RMs may be locked.

        * `global abort` will be sent, or the first phase will be repeated.

    * If last record was `global commit/abort`, the second phase needs to be repeated.

    * If last record was `complete`, everything is fine.

* Message loss: handled by timeouts, which cause a `global abort` in the first phase, or a retransmission in the second phase.


#### Optimizations

* **Presumed abort protocol**: if in doubt during a RM recovery, and TM has no information, `abort` is returned.

    * Some synchronous record writes can be avoided.

* **Read-only optimization**: if an RM only needs to read, it will not influence the transaction's result - it can be ignored during second phase.


### Other commit protocols

* The biggest issue with the 2-phase protocol is that an RM can become stuck if the TM drops.

    * The following protocols don't have this issue but are less performant.

#### 4-phase commit protocol

* The TM process can be replicated by a **backup process** on a different node.

    * On every phase, the TM first communicates with the backup, then with the RMs.


#### 3-phase commit protocol

* After receiving `ready` from every RM, the TM has an additional **pre-commit** state.

    * If the TM drops during that state, any RM can become the TM, because every RM has to be `ready`.

* Unusable in practice due to widened uncertainty interval and atomicity issues in case of network partitioning.


#### Paxos commit

* More general goal: have nodes "agree" on a specific value in case of malfunction.

* Three node categories:

    * Proponent.

    * Acceptor.

    * Receiver.

* Three phases:

    1. Election of a coordinator.

    2. Acceptors agree on a value.

    3. The value is propagated to receivers.

* Algorithm:

    1. The coordinator sends $n$ `prepare` messages to participants.

    2. Every participant sends `ready` to coordinator and to $f$ acceptors.

    3. Every acceptor sends its state using $f$ messages.

    4. Coordinator and acceptors are $f + 1$ nodes that know the state of the transaction. Any malfunction in $f$ is not a problem.


#### X-Open DTP

* Guarantees interoperability of transactions on different DBMSs.

* Two main interfaces:

    1. **TM-interface**: between client and TM. 

        * `tm_xxx` functions.

    2. **XA-interface**: between TM and RM.

        * Database vendors must guarantee XA-interface availability.

        * `xa_xxx` functions.

* Features:

    * RMs are passive. All control is in TM, which uses RPCs to enable RM functions.

    * Uses 2-phase commit with aforementioned optimizations.

    * **Heuristical decisions** are taken, which can harm atomiticy *(notifying clients)*.


## DBMS replication

* A **data replicator** handles replication and **synchronization** between copies.

    * Copies are updated asynchronously *(no commit protocols)*.

* Replication data can be **batched** and reconciled with the copies all at once.

* **Multidatabase systems**: tree hierarchies of **dispatchers** and multiple DBs behind a single interface.