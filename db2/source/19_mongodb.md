


# MongoDB

## Background

* **MongoDB** is a document-oriented NoSQL DBMS.

* Uses **BSON** format.

* Schema-less.

* **No transactions** and **no joins**.


## Basics

* A MongoDB **instance** contains **databases**.

* A database contains **collections**.

    * Conceptually similar to tables in SQL.

* A collection contains **documents**.

    * Conceptually similar to records in SQL.

    * Every document has an **unique key**.

* A document contains **fields**.

* **Indexing** support.

    * Uses **B-trees**.

### Examples

* Documents:

    * ```json
        user = {
            name: “Z",
            occupation: "A scientist",
            location: “New York"
            }
    ```

* Collections:

    * ```json
        { 
            "_id": ObjectId("4efa8d2b7d284dad101e4bc9"),
            "Last Name": "DUMONT",
            "First Name": "Jean",
            "Date of Birth": "01-22-1963" 
        }
    ```

    * ```json
        { 
            "_id": ObjectId("4efa8d2b7d284dad101e4bc7"),
            "Last Name": "PELLERIN",
            "First Name": "Franck",
            "Date of Birth": "09-19-1983",
            "Address": "1 chemin des Loges",
            "City": "VERSAILLES" 
        }
    ```


* Queries: 

    * ```json
    db.users.find( {last_name: 'Smith'} )
    ```

    * ```json
    db.users.find( {age: {$gte: 23} } )
    ```

    * ```json
    db.users.find( {age: {$in: [23,25]} } )
    ```
