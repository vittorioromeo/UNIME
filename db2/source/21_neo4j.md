



# Neo4J

## Graph databases

* Schema-less.

* Efficient storage of semi-structured data.

* No **O/R mismatch**.

    * Natural to map a graph to OOP language.

* Express queries as traversals.

* Express graph-related problems.

    * *Example:* does a path exist between A and B?

## Features

* Both **nodes** and **edges** can contain **properties**.

* Edges are **relationships**:

    * They have a start node and end node.

    * Have a relationship type.

    * Can have properties.

* **ACID**.
    
    * Transaction support.

* Query language: **Cypher**.

* Bad horizonal scalability:

    * Read-only scalability: all writes go to master, then fan out.

## Examples

```sql
CREATE (p1:Profilo1)

CREATE (m:Movie:Cinema:Film:Picture)

CREATE (p1:Profilo1)-[relazione1:LIKES]->(p2:Profilo2)

MATCH (emp:Employee) RETURN emp.empid,emp.name,emp.salary,emp.deptno

MATCH (emp:Employee) WHERE emp.name = 'Abc' RETURN emp

MATCH (emp:Employee) WHERE emp.name = 'Abc' OR emp.name = 'Xyz' RETURN emp

MATCH (cust:Customer),(cc:CreditCard)
WHERE cust.id = "1001" AND cc.id= "5001" 
CREATE (cust)-[r:DO_SHOPPING_WITH{shopdate:"12/12/2014"}]->(cc) 
RETURN r

MATCH (cc:CreditCard)-[r]-(c:Customer)RETURN r 

MATCH (cc: CreditCard)-[rel]-(c:Customer) DELETE cc,c,rel

MATCH (e: Employee) DELETE e
```