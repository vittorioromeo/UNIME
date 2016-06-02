


# Distributed systems

## General information

* A distributed system is a **software** that makes **a collection of independent machines** appear as **a single coherent system**.

    * Achieved thanks to a **middleware**.

* Goals:

    * Making resource available.

    * Distribution **transparency**.

    * **Openness** and **scalability**.

### Transparency

| Type          | Description                                           |
|---------------|-------------------------------------------------------|
| Access        | Hides data access                                     |
| Location      | Hides data locality                                   |
| Migration     | Hides ability of a system to change object location   |
| Relocation    | Hides system ability to move object bound to client   |
| Replication   | Hides object replication                              |
| Concurrency   | Hides coordination between objects                    |
| Failure       | Hides failure and recovery                            |

* Hard to fully achieve.

    * Users may live in different continents.

    * Networks are unreliable.

    * Full trasparency is costly.

### Openness

* Conformance to well-defined interfaces.

* Portability and interoperability.

* Heterogeneity of underlying environments.

* Requires support for **policies**.

* Provides **mechanisms** to fulfill policies.

### Scalability

* **Size**: number of users/processes.

* **Geographical**: maximum distance between nodes.

* **Administrative**: number of administrative domains.

* Techniques to achieve scalability:

    * Hide communication latencies.

        * Use **asynchronous** communication.

        * Use **separate response handlers**.

    * Distribution.

        * Decentralized **DNS** and information systems.

        * Try to compute as much as possible on clients.

    * Replication/caching.

* Issue: **inconsistency** and **global synchronization**.


## Types

### Distributed Computing Systems

* **HPC** *(high-performance computing)*.

* Cluster computing:

    * **Homogeneous** LAN-connected machines.

        * Master node + compute nodes.

* Grid computing:

    * **Heterogeneous** WAN-connected machines.

    * Usually divided in **virtual organizations**.


### Distributed Information Systems

* **Transaction**-based systems.

    * **Atomicity**.

    * **Consistency**.

    * **Isolation**: no interference between concurrent transaction.

    * **Durability**: changes are permanent.

* **TP Monitors** *(transaction processing monitors)* coordinate execution of a distributed transaction.

    * **Communication middleware** is required to separate applications from databases.

        * **RPC** *(remote procedure call)*.

        * **MOM** *(message-oriented middleware)*.


### Distributed Pervasive Systems

* Small nodes, often **mobile** or **embedded**.

* Requirements:

    * **Contextual change**.

    * **Ad-hoc composition**.

    * **Sharing by default**.

* Examples:

    * Home systems.

    * Electronic health systems.

    * Sensor networks.



## Architectures

### Styles and models

* Architectural styles:

    * **Layered**: used for client-server systems.

    * **Object-based**: used for distributed systems.

* Decoupling models:

    * **Publish/subscribe**: uses **event bus**, decoupled in space.

    * **Shared dataspace**: used shared persistent data space, decoupled both in space and time.


### Centralized architectures

* Client-server.

* Three-layered view:

    * User-interface layer.

    * Processing layer.

    * Data layer.

* Multi-tiered architecture:

    * Single-tiered: dumb terminal/mainframe.

    * Two-tiered: client-server.

    * Three-tiered: each layer on separate machine.


### Decentralized architectures

* **P2P** *(peer-to-peer)*:

    * P2P architectures are **overlay networks**: application-level multicasting.

    * **Structured**: nodes follow a specific data structure.

        * Example: ring, kd-tree.

    * **Unstructured**: nodes choose random neighbors.

        * Example: random graph.

            * Each node has a **partial view** of the network which is shared with random nodes selected periodically, along with data.

    * **Hybrid**: some nodes are special *(and structured)*.

* Topology management:

    * 2 layers: structured and random.

        * Promote some nodes depending on their services.

        * Torus construction: create $N * N$ grid, keep only **nearest neighbors** via distance formula.

        * **Superpeers:** few specific nodes.

            * Examples: indexing, coordination, connection setup.

* Hybrid architectures *(P2P + client-server)*:

    * **CDNs**: edge-server architectures.

    * **BitTorrent**: tracker and peers.


### Architectures versus middleware

* Sometimes the middleware needs to **dyamically adapt its behavior** to distributed application/systems.

    * **Interceptors** can be used.

    * **Adaptive middleware**:

        * Separation of concerns.

        * Computational reflection *(self runtime inspection)*.

        * Component-based design.



### Self-managing distributed systems

* Self-*x* operations:

    * Configuration.

    * Management.

    * Healing.

    * Optimization.

* **Feedback control model**.

    * Example: globule *(collaborative CDN driven by cost model)*.