



# Cloud computing

TODO: merge previous chapter into this one or viceversa?

TODO: add cloud federations to cloud models 

## Definitions

* **Cloud computing** describes a class of network-based computing:

    * A collection/group of networked hardware, software and infrastructure *(platform)*.

    * Uses the Internet for communication/transport, providing hardware and software services to client.

* The complexity of the platforms is hidden behind simple **APIs**.

* Characteristics:

    * **Remotely hosted**.

    * **Ubiquitous**: services/data available from anywhere.

    * **Commodified**: pay for what you want/need.

    * Common characteristics:

        * Massive scale.

        * Resilient computing.

        * Homogeneity.

        * Geographic distribution.

        * Virtualization.

        * Service-orientation.

        * Low-cost.

        * Security.

    * Essential characteristics:

        * On-demand self-service.

        * Broad network access.

        * Elasticity.

        * Resource pooling.

        * Measured service.

## Service models

### Layers

* From application-focused to infrastructure-focused:

    1. Services.

    2. Application.

    3. Development.

    4. Platform.

    5. Storage.

    6. Hosting.


### IaaS

* Provides hardware.

#### Virtualization

* The basis of IaaS.

* **Virtual workspaces**: abstraction over the execution environment.

    * Has specific resource quota and software configuration.

* Implemented on **VMs** *(virtual machines)*.

    * Abstraction of the physical host.

    * Advantages:

        * OS flexibility. Easier deployment.

        * Versioning/backups/migrations.

* A **VMM** *(virtual machine monitor, or hypervisor)* is used to manage multiple VMs on a single machine.


### PaaS

* Deploys user-created applications.

* Highly-scalable architecture.


### SaaS

* Provides applications.

* *Examples:* Facebook apps, Google apps.

#### Maturity model

* **Level 1**: ad-hoc/custom. One instance per customer.

* **Level 2**: configurable per customer.

* **Level 3**: configurable and multi-tenant-efficient.

* **Level 4**: scalable *(uses load balancer)* level 3.