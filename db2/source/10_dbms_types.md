
# DBMS types

## Relational DBMSs

* Formally introduced by **Codd** in 1970.

* ANSI standard: **SQL**.

* Composed of many relations in form of **2D tables**, containing **tuples**.

    * **Logical view**: data organized in tables.

    * **Internal view**: stored data.

    * Rows *(tuples)* are **records**.

    * Columns *(fields)* are **attributes**.

        * They have specific **data types**.

* **Constraints** are used to restrict stored data.

* **SQL** is divided in **DDL** and **DML**.

### Disadvantages

* Lack of flexibility: all processing is based on values in fields of records.

* Inability to handle complex types and complex interrelationships.



## Object-oriented DBMSs

* Integrated with an OOP language.

* Supports:

    * Complex data types.

    * Type inheritance.

    * Object behavior.

* Objects have an **OID** *(object identifier)*.

* **ADTs** *(abstract data types)* are used for **encapsulation**.

* OODBMSs were standardized by **ODMG** *(Object Data Management Group)*.

    * Object model, **ODL**, **OQL** and OOP language bindings.

* **OQL** resembles **SQL**, with additional features *(object identity, complex types, inheritance, polymorphism, ...)*.


### Disadvantages

* Poor performance. Queries are hard to optimize.

* Poor scalability.

* Problematic change of schema.

* Dependence from OOP language.

### Advantages

* Composite objects and relations.

* Easily manageable class hierarchies.

* Dynamic data model

* No primary key management.



## Object-relational DBMSs

* Hybrid solution, expected to perform well.

* Features:

    * Base datatype extension *(inheritance)*.

    * Complex objects.

    * Rule systems.