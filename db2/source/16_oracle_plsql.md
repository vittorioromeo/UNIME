


# Oracle and PL/SQL

* **Oracle Database** is an object-relational database management system *(ORDBMS)*.

* **PL/SQL** is also known as **Embedded SQL**.

* More powerful than pure **SQL**:

    * Has **iteration**, **branching**, **cursors**, **blocks**, **stored procedures**, and more.


## Basic structure

```sql
DECLARE
    -- ...
BEGIN
    -- ...
EXCEPTION 
    -- ...
END; 
```
### Server output

* Execute `set serveroutput on` before running.

```sql
BEGIN
    DBMS_OUTPUT.PUT_LINE('Hello world!');
END;
```

### Example 

```sql
DECLARE
    v_id INTEGER;
    v_empno NUMBER;

BEGIN
    v_id := 1234567;
    SELECT EMPNO
    INTO v_empno
    FROM EMP
    WHERE empno = v_id;
    DBMS_OUTPUT.PUT_LINE('Value is ' || v_empno);

EXCEPTION
    WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE('No record exists');

END;
```




## Variables

* Common data types:

    * `NUMBER`.

    * `DATE`.

    * `INTEGER`.

    * `VARCHAR2`.

    * `CHAR`.

    * `BOOLEAN`.



## `SELECT INTO` example

```sql
DECLARE
    v_job emp.job%TYPE;
    v_sal emp.sal%TYPE;
    v_empno emp.empno%TYPE;

BEGIN
    v_empno := 1234567;
    SELECT job, sal
    INTO v_job,v_sal
    FROM emp
    WHERE empno = v_empno;

END;
```


## `IF` example

```sql
DECLARE
    -- ...

BEGIN
    -- ...
    IF v_dept = 10 THEN
        v_commision := 5000;
    ELSIF v_dept = 20 THEN
        v_commison := 5500;
    ELSIF v_dept = 30 THEN
        v_commison := 6200;
    ELSE
        v_commision := 7500;
    END IF;
    -- ...

END;
```


## Loops

* `LOOP`, `EXIT WHEN`, `END LOOP`.

* `FOR`, `IN`, `LOOP`, `END LOOP`.

* `WHILE`, `LOOP`, `END LOOP`.


### `LOOP` example

```sql
LOOP
    INSERT INTO dept(deptno)
    VALUES(v_deptno);
    v_counter := v_counter + 1;
    v_deptno := v_deptno + 10;
    EXIT WHEN v_counter > 5;
END LOOP;
```


### `FOR` example

```sql
FOR v_counter IN 1..5 LOOP
    INSERT INTO dept(deptno)
    VALUES(v_deptno);
    v_deptno := v_deptno + 10;
END LOOP;
```


### `WHILE` example

```sql
v_counter := 1;
WHILE v_counter <= 5 LOOP
    INSERT INTO dept(deptno)
    VALUES(v_deptno);
    v_deptno := v_deptno + 10;
END LOOP;
```


## Procedures

### Syntax

```sql
CREATE OR REPLACE PROCEDURE /*name*/(/*parameters*/) IS
    -- local variables

BEGIN
    -- ...

EXCEPTION
    -- ...

END;
```

* Parameters can be `IN`, `OUT` or `IN OUT`.


### Example

```sql
CREATE OR REPLACE PROCEDURE proc_test(p_empno IN VARCHAR2) IS
    v_job EMP.job%TYPE;
    v_sal EMP.sal%TYPE;

BEGIN
    SELECT job, sal
    INTO v_job,v_sal
    FROM emp
        WHERE empno = p_empno;
    DBMS_OUTPUT.PUT_LINE('job is '||v_job);

EXCEPTION
    WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('ERROR...');

END;
```


## Functions

### Syntax

```sql
CREATE OR REPLACE FUNCTION /*name*/(/*parameters*/) 
RETURN /*datatype*/ IS
    -- local variables

BEGIN
    -- ...

EXCEPTION
    -- ...

END;
```

* Paremeters can only be `IN`.

* Returns a single value.


### Example: swapping values

```sql
CREATE OR REPLACE FUNCTION swap(a IN OUT integer, b IN OUT integer)
    RETURN integer
IS
    temp integer;
BEGIN
    temp := a;
    a := b;
    b := temp;
    RETURN a;
END;
```


## Packages

### Specification example

```sql
CREATE OR REPLACE PACKAGE emp_info IS
    
    v_count INTEGER;
    
    PROCEDURE insert_record(  p_empno IN NUMBER
                            , p_ename IN VARCHAR2
                            , p_job IN VARCHAR2
                            , p_sal IN NUMBER
                            , p_comm IN NUMBER
                            , p_deptno IN VARCHAR2);

    PROCEDURE delete_record(p_empno IN NUMBER);

    FUNCTION sum_dept_sal( p_deptno IN dept.deptno%TYPE) RETURN is dept.sal%TYPE;

END emp_info;
```


### Body definition syntax

```sql
CREATE OR REPLACE PACKAGE BODY emp_info IS

    -- define declared procedures and functions

END emp_info;
```



## Triggers

### Syntax example

```sql
CREATE OR REPLACE TRIGGER del_emp( p_empno emp.empno%TYPE)
BEFORE DELETE ON emp
FOR EACH ROW
BEGIN
    INSERT INTO emp_audit
    VALUES(p_empno, USER, sysdate);

END;
```


## Cursors

* A **cursor** is a pointer to a row.

### Syntax example

```sql
DECLARE
    CURSOR c_emp IS
    SELECT empno, ename, job
    FROM emp
    WHERE deptno = 20;

BEGIN
    FOR v_c IN c_emp LOOP
        DBMS_OUTPUT.PUT_LINE(v_c.ename);
    END LOOP;

END;  
```

### Example: copying between tables

```sql
DECLARE 
    CURSOR c IS
    SELECT CF, Productivity, Salary 
    FROM People;
    
    temp_bonus integer; 

BEGIN
    FOR i IN c LOOP
        IF ( i.Productivity >= 20 ) THEN
            temp_bonus := 15;
        ELSIF ( i.Productivity >= 10 ) THEN
            temp_bonus := 10;
        ELSE
            temp_bonus := 5;
        END IF;

        INSERT INTO PeopleUpgrade(CF, Productivity, Salary, Bonus)
        VALUES(i.CF, i.Productivity, i.Salary, temp_bonus);

    END LOOP;
END;
```


## Dynamic SQL

```sql
BEGIN
    EXECUTE IMMEDIATE 'CREATE TABLE tt(id NUMBER(3)'
END;
```