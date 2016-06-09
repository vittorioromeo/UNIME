


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

## Examples

### Basic

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

### Complex

```json
db.createCollection(miaCollection, options)
db.COLLECTION_NAME.drop()

db.miaCollection.insert({name: Mario, sesso:’M’, peso: 450})
db.miaCollection.find({sesso: ’m’,peso: {$gt: 700}})
db.miaCollection.update({name: 'Mario'}, {$set: {peso: 590}})
db.miaCollection.find().sort({peso: -1})
db.miaCollection.count({peso: {$gt: 50}})

db.employees.insert({
    _id: ObjectId(”4d85c7039ab0fd70a117d734”),
    name: ’Ghanima’,
    scores:[],
    latlong: [40.0,70.0],
    family: 
        {mother: ’Chani’,
        father: ’Paul’,
        brother: ObjectId(”4d85c7039ab0fd70a117d730”)
        }
    })
db.employees.find({’family.mother’: ’Chani’})
db.employees.update({ _id: 1 }, { $push: { scores: 89 } }
db.employees.find({latlong:{$near: { [40,70], $minDistance: 
    1000,$maxDistance: 5000 }}})
```

